//
// Created by AwokenOwen on 6/11/26.
//
#pragma once
#include "Component.h"
#include "LinearMath.h"
#include "ResourceManager.h"
enum LightType
{
    DIR = 0,
    POINT = 1,
    SPOT = 2
};
class LightComponent : public Component
{
    friend class ResourceManager;
public:
    explicit LightComponent(Object* parent)
        : Component(parent)
    {
    }
    [[nodiscard]] LightType getLightType() const
    {
        return m_type;
    };

    void setShadowMap(FrameBuffer shadowMap);
    [[nodiscard]] unsigned int getShadowMap() const;
    unsigned int getShadowBuffer() const;
private:
    LightType m_type{DIR};

    // For all lights
    Vector3 m_direction{0.0f, -0.707, -0.707};
    Vector3 m_color{1.0f, 1.0f, 1.0f};
    float m_power{1.0f};

    // For spotlights only
    float m_radius{-1.0f};

    FrameBuffer m_shadowMap{};

    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
};
