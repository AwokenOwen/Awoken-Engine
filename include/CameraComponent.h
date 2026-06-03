//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Math.h"

#define PERSPECTIVE true
#define ORTHOGRAPHIC false

class CameraComponent : public Component{
    friend class ResourceManager;
public:
    explicit CameraComponent(Object* parent);
    ~CameraComponent() override = default;

    void setActiveState(bool active) override;

    [[nodiscard]] Matrix4 getViewMatrix() const;
    [[nodiscard]] Matrix4 getProjectionMatrix() const;

    void setProjectionType(bool projectionType);

    bool m_main{false};

private:
    bool m_projectionType{true};

    float m_fov{toRadians(90.0f)};
    float m_near{0.001f};
    float m_far{1000.0f};

    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;

    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
};
