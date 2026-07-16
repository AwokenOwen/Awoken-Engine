//
// Created by AwokenOwen on 7/16/26.
//
#pragma once
#include "LinearMath.h"
#include "Event.h"
#include <vector>

#include "LogManager.h"
#include "nlohmann/json.hpp"

class ComponentUI;
class ObjectUI
{
    friend class WorldManager;
    friend struct Scene;
    friend class ResourceManager;

public:
    ObjectUI() = default;
    ~ObjectUI() = default;

    [[nodiscard]] Vector2 getLocalPosition();
    void setLocalPosition(const Vector2& localPosition);

    [[nodiscard]] float getLocalRotation() const;
    void setLocalRotation(float localRotation);

    [[nodiscard]] Vector2 getLocalScale();
    void setLocalScale(const Vector2& localScale);

    [[nodiscard]] Matrix4 getLocalMatrix();

    [[nodiscard]] Vector2 getWorldPosition();
    void setWorldPosition(const Vector2& worldPosition);

    [[nodiscard]] float getWorldRotation();
    void setWorldRotation(float worldRotation);

    [[nodiscard]] Vector2 getWorldScale();
    void setWorldScale(const Vector2& worldScale);

    [[nodiscard]] Matrix4 getWorldMatrix();

    [[nodiscard]] ObjectUI* getParent() const;

    void setActiveState(bool active = true);

    void setComponentActiveState(ComponentUI* component, bool active);

    template<typename T>
    T* addComponent();

    template<typename T>
    T* getComponent();

    EVENT_ACCESSORS(StartEvent)
    EVENT_ACCESSORS(UpdateEvent)
    EVENT_ACCESSORS(EnableEvent)
    EVENT_ACCESSORS(DisableEvent)
    EVENT_ACCESSORS(DestroyEvent)

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

    Vector2 m_localPosition{};
    float m_localRotation{};
    Vector2 m_localScale{1,1};

    ObjectUI* p_parent{};
    std::vector<ObjectUI*> m_children{};
    std::vector<ComponentUI*> m_components{};

    bool m_activeState{true};

    Event<> StartEvent{};
    Event<> UpdateEvent{};
    Event<> EnableEvent{};
    Event<> DisableEvent{};
    Event<> DestroyEvent{};

    nlohmann::json toJson();
    static ObjectUI* fromJson(const nlohmann::json& j);
};

template<typename T> T* ObjectUI::addComponent() {
    static_assert(std::is_base_of_v<ComponentUI, T>, "T must derive from Component");

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

template<typename T> T* ObjectUI::getComponent() {
    static_assert(std::is_base_of_v<ComponentUI, T>, "T must derive from ComponentUI");

    for (auto c: m_components) {
        if (dynamic_cast<T*>(c))
        {
            return dynamic_cast<T*>(c);
        }
    }
    return nullptr;
}