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
    Object() = default;
    virtual ~Object() = default;

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

    void setComponentActiveState(Component* component, bool active);

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

    EVENT_ACCESSORS(StartEvent)
    EVENT_ACCESSORS(UpdateEvent)
    EVENT_ACCESSORS(EnableEvent)
    EVENT_ACCESSORS(DisableEvent)
    EVENT_ACCESSORS(DestroyEvent)

    void Translate(const Vector3 &translation);

protected:
    /**
     * @brief Called every frame
     */
    void update();
    /**
     * @brief Called when the object's active state is set to true
     */
    void enable();
    /**
     * @brief Called when the object's active state is set to false
     */
    void disable();
    /**
     * @brief Called when the object is destroyed
     */
    void destroy();
    /**
     * @brief Called during the termination of the engine to free everything
     */
    void end();

private:
    std::string m_name;

    Vector3 m_localPosition{};
    Quaternion m_localRotation{};
    Vector3 m_localScale{1,1,1};

    Object* p_parent{};
    std::vector<Object*> m_children{};
    std::vector<Component*> m_components{};

    bool m_activeState{true};

    Event<> StartEvent{};
    Event<> UpdateEvent{};
    Event<> EnableEvent{};
    Event<> DisableEvent{};
    Event<> DestroyEvent{};

    nlohmann::json toJson();
    static Object* fromJson(const nlohmann::json& j);
};

template<typename T> T* Object::addComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    for (auto c: m_components) {
        if (dynamic_cast<T*>(c))
        {
            Log.logError("Component already added... returning unique component");
            return dynamic_cast<T*>(c);
        }
    }
    auto newComponent = new T(this);
    this->m_components.push_back(newComponent);
    return newComponent;
}

template<typename T> T* Object::getComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    for (auto c: m_components) {
        if (dynamic_cast<T*>(c))
        {
            return dynamic_cast<T*>(c);
        }
    }
    return nullptr;
}
