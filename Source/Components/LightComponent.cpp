//
// Created by AwokenOwen on 6/11/26.
//

#include "LightComponent.h"

#include "ResourceManager.h"

void LightComponent::setShadowMap(FrameBuffer shadowMap)
{
    m_shadowMap = shadowMap;
}

unsigned int LightComponent::getShadowMap() const
{
    return m_shadowMap.m_colorBuffer;
}

unsigned int LightComponent::getShadowBuffer() const
{
    return m_shadowMap.m_id;
}

void LightComponent::start()
{
}

void LightComponent::update()
{
}

void LightComponent::enable()
{
}

void LightComponent::disable()
{
}

nlohmann::json LightComponent::toJson()
{
    nlohmann::json j;

    j["Type"] = "Light";
    j["LightType"] = m_type;
    j["Direction"] = m_direction.toJson();
    j["Color"] = m_color.toJson();
    j["Power"] = m_power;

    return j;
}

void LightComponent::fromJson(nlohmann::json j)
{
    m_type = j["LightType"];
    m_direction = Vector3::fromJson(j["Direction"]);
    m_color = Vector3::fromJson(j["Color"]);
    m_power = j["Power"];

    Resource.registerLight(this);
}
