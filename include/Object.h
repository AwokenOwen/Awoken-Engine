//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include "LinearMath.h"
#include "Event.h"
#include <vector>

#include "LogManager.h"
#include "nlohmann/json.hpp"

class Component;
class Object {
    friend class WorldManager;
    friend struct Scene;
    friend class ResourceManager;
public:
    /**
     * @return the local position of the object
     */
    [[nodiscard]] Vector3 getLocalPosition() const;
    /**
     * @param position the new local position of the object
     */
    void setLocalPosition(const Vector3 &position);
    /**
     * @return The local rotation of the object as a quaternion
     */
    [[nodiscard]] Quaternion getLocalRotation() const;
    /**
     * @param rotation the new local rotation of an object as a quaternion
     */
    void setLocalRotation(const Quaternion &rotation);
    /**
     * @return The local scale of an object
     */
    [[nodiscard]] Vector3 getLocalScale() const;
    /**
     * @param scale The new local scale of the object
     */
    void setLocalScale(const Vector3 &scale);
    /**
     * @return The local model matrix of an object
     */
    [[nodiscard]] Matrix4 getLocalMatrix() const;

    /**
     * @return the world position of the object
     */
    [[nodiscard]] Vector3 getWorldPosition() const;
    /**
     * @param position the new world position of the object
     */
    void setWorldPosition(const Vector3 &position);
    /**
     * @return The world rotation of the object as a quaternion
     */
    [[nodiscard]] Quaternion getWorldRotation() const;
    /**
     * @param rotation the new world rotation of an object as a quaternion
     */
    void setWorldRotation(const Quaternion &rotation);
    /**
     * @return The world scale of an object
     */
    [[nodiscard]] Vector3 getWorldScale() const;
    /**
     * @param scale The new world scale of the object
     */
    void setWorldScale(const Vector3 &scale);
    /**
     * @return The world model matrix of an object
     */
    [[nodiscard]] Matrix4 getWorldMatrix() const;

    /**
     * @return The world forward vector of the object
     */
    [[nodiscard]] Vector3 getWorldForward() const;

    /**
     * @return The world right vector of the object
     */
    [[nodiscard]] Vector3 getWorldRight() const;

    /**
     * @return The world up vector of the object
     */
    [[nodiscard]] Vector3 getWorldUp() const;

    /**
     * @return The parent of the object, nullptr if no parent
     */
    [[nodiscard]] Object* getParent() const;

    /**
     * @return The active state of the object
     */
    [[nodiscard]] bool getActiveState() const;
    /**
     * @brief Sets the active state and adds it to the correct event with the world manager
     *
     * @param active the new active state
     */
    void setActiveState(bool active = true);

    /**
     * @brief Adds a new component type to an object, only one instance of a component is allowed on an object
     *
     * @tparam T The type of component
     * @return The newly added component or an already added one of the same type
     */
    template<typename T>
    T* addComponent();

    /**
     * @brief Gets an already added component from an object, if not already added return nullptr
     *
     * @tparam T The type of the component
     * @return The component on the object or nullptr if it doesn't exist
     */
    template<typename T>
    T* getComponent();

    /**
     * @brief Function that translates the object through local space
     *
     * @param delta The change in position
     */
    void Translate(const Vector3 &delta);
    /**
     * @brief Function that rotates the object through local space
     *
     * @param delta The change in rotation
     */
    void Rotate(const Quaternion &delta);
    /**
     * @brief Function that scales the object through local space
     *
     * @param delta The change in scale
     */
    void Scale(const Vector3 &delta);

    /**
     * @brief Getter for the scene that owns this object
     *
     * @return The scene this object is from
     */
    [[nodiscard]] Scene* getScene() const;

    void destroy();

    EVENT_ACCESSORS(StartEvent)
    EVENT_ACCESSORS(UpdateEvent)
    EVENT_ACCESSORS(EnableEvent)
    EVENT_ACCESSORS(DisableEvent)

protected:
    /**
     * @brief Called every frame
     */
    void update();

private:
    /**
     * @brief default constructor
     */
    Object() = default;
    /**
     * @brief default deconstructor
     */
    virtual ~Object() = default;

    /**
     * @brief The name of the object in the scene file
     */
    std::string m_name;
    /**
     * @brief The local position of the object
     */
    Vector3 m_localPosition{};
    /**
     * @brief The local rotation of the object
     */
    Quaternion m_localRotation{};
    /**
     * @brief The local scale of the object
     */
    Vector3 m_localScale{1,1,1};
    /**
     * @brief The model matrix of the object
     */
    Matrix4 m_localMatrix{};

    /**
    * @brief The cached world matrix
    */
    mutable Matrix4 m_worldMatrix{};
    /**
    * @brief The dirty flag for when to update the cached world matrix
    */
    mutable bool m_dirtyFlag{};
    /**
    * @brief Function used to set the dirty flag of this object and all child objects
    */
    void setDirtyFlag() const;

    /**
     * @brief The scene the object belongs to
     */
    Scene* m_scene{};
    /**
     * @brief The parent object of this object
     */
    Object* p_parent{};
    /**
     * @brief The list of children of this object
     *
     * Not sure if this needs to be removed or not
     */
    std::vector<Object*> m_children{};
    /**
     * @brief The list of the components attached to this object
     */
    std::vector<Component*> m_components{};

    /**
     * @brief The active state of the object
     */
    bool m_activeState{true};

    /**
     * @brief The start event for the components of this object
     */
    Event<> StartEvent{};
    /**
     * @brief The update event for the components of this object
     */
    Event<> UpdateEvent{};
    /**
     * @brief The enable event for the components of this object
     */
    Event<> EnableEvent{};
    /**
     * @brief The disable event for the components of this object
     */
    Event<> DisableEvent{};

    /**
     * @brief Function to convert an Object into a JSON file
     *
     * @return JSON object with the object data
     */
    nlohmann::json toJson();
    /**
     * @brief Function used to make an object from the JSON in the scene file
     *
     * @param j JSON object with all the data to build an object
     * @return A new object
     */
    static Object* fromJson(const nlohmann::json& j);
};

template<typename T> T* Object::addComponent() {
    // Require T to be a Component
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    // Loop through all components
    for (auto c: m_components) {
        // Check if the component is already added
        if (dynamic_cast<T*>(c))
        {
            // Log the warning
            Log.logWarning("Component already added... returning unique component");
            // Return the component already added
            return dynamic_cast<T*>(c);
        }
    }
    // Create the new component and attach it to the object
    auto newComponent = new T(this);
    // Add the component to the list
    this->m_components.push_back(newComponent);
    // Return the new component
    return newComponent;
}

template<typename T> T* Object::getComponent() {
    // Require T to be a Component
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    // Loop through all components
    for (auto c: m_components) {
        // If it's the component being looking for
        if (dynamic_cast<T*>(c))
        {
            // Return that component
            return dynamic_cast<T*>(c);
        }
    }
    // If there is no component log error
    Log.logWarning("No component of that type...");
    // Return nullptr
    return nullptr;
}
