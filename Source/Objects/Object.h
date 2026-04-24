//
// Created by awokenowen on 4/13/26.
//

#pragma once
#include "Math.h"
#include "LogManager.h"
#include "Event.h"
#include <vector>

#include "nlohmann/json.hpp"

class Component;
class Object {
    friend class WorldManager;
    friend struct Scene;
public:
    Object();
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
     * @return The active state of the object
     */
    [[nodiscard]] bool getActiveState() const;
    /**
     * @brief Sets the active state and adds it to the correct event with the world manager
     *
     * @param activeState the new active state
     */
    void setActiveState(bool activeState = true);

    void addChild(Object* child);

private:
    void start();
    void update();
    void enable();
    void disable();
    void destroy();
    void end() const;

    std::string name;

    Vector3 m_localPosition{};
    Quaternion m_localRotation{};
    Vector3 m_localScale{1,1,1};

    Object* p_parent{};
    std::vector<Object*> m_children{};
    std::vector<Component*> m_components{};

    bool m_activeState{true};

    Event<> m_startEvent{this};
    Event<> m_updateEvent{this};
    Event<> m_enableEvent{this};
    Event<> m_disableEvent{this};
    Event<> m_destroyEvent{this};

    nlohmann::json toJson() {
        nlohmann::json j;

        j["Name"] = name;

        j["ActiveState"] = m_activeState;

        j["Position"]["x"] = m_localPosition.x;
        j["Position"]["y"] = m_localPosition.y;
        j["Position"]["z"] = m_localPosition.z;

        j["Rotation"]["x"] = m_localRotation.x;
        j["Rotation"]["y"] = m_localRotation.y;
        j["Rotation"]["z"] = m_localRotation.z;
        j["Rotation"]["w"] = m_localRotation.w;

        j["Scale"]["x"] = m_localScale.x;
        j["Scale"]["y"] = m_localScale.y;
        j["Scale"]["z"] = m_localScale.z;

        for (auto c : m_components) {
            // do later
        }

        int untitledNumber = 0;
        std::vector<nlohmann::json> temp;
        for (auto& object : m_children) {
            if (object->name == "") {
                object->name = "Untitled_" + std::to_string(untitledNumber++);
            }
            temp.push_back(object->toJson());
        }

        j["Children"] = temp;
        return j;
    }

    static Object* fromJson(const nlohmann::json& j) {
        auto a = new Object();

        a->m_activeState = j["ActiveState"];

        a->name = j["Name"];

        a->m_localPosition = Vector3(j["Position"]["x"].get<double>(), j["Position"]["y"].get<double>(),j["Position"]["z"].get<double>());
        a->m_localRotation = Quaternion(j["Rotation"]["x"].get<double>(), j["Rotation"]["y"].get<double>(), j["Rotation"]["z"].get<double>(), j["Rotation"]["w"].get<double>());
        a->m_localScale = Vector3(j["Scale"]["x"].get<double>(), j["Scale"]["y"].get<double>(), j["Scale"]["z"].get<double>());

        std::vector<nlohmann::json> children = j["Children"];

        for (auto c : children) {
            a->addChild(fromJson(c));
        }

        return a;
    }
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
    return new T();
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
