//
// Created by AwokenOwen on 6/11/26.
//

#include "LightComponent.h"

#include "CameraComponent.h"
#include "Object.h"
#include "ResourceManager.h"

std::vector<Matrix4> LightComponent::getLightSpaceMatrix() const
{
    if (m_type != DIR)
    {
        Log.logError("No code for point shadows made yet");
        return {};
    }

    constexpr float near_plane = 1.0f, far_plane = 10.0f;
    const auto lightView = Matrix4::lookAt(Vector3(0, 1, 0), Vector3(0,0,0), Vector3::up());
    const auto lightProjection = Matrix4::makeOrthographicMatrix(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    const auto lightSpaceMatrix = lightProjection * lightView;

    auto v = std::vector<Matrix4>();
    v.push_back(lightSpaceMatrix);
    return v;
}

void LightComponent::activateShadowMap() const
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap.m_id);
    glClear(GL_DEPTH_BUFFER_BIT);
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
}

void LightComponent::destroy()
{
    //Make sure to remove light
    delete this;
}
