//
// Created by AwokenOwen on 6/11/26.
//

#include "LightComponent.h"

#include "CameraComponent.h"
#include "Object.h"
#include "ResourceManager.h"

void LightComponent::activateShadowMap() const
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap.m_id);
    glClear(GL_DEPTH_BUFFER_BIT);
}

Matrix4 LightComponent::getLightViewProjectionMatrix() const
{
    Vector3 center{};
    const auto corners = getFrustumCornersWorldSpace(Resource.getMainCamera()->getPerspectiveMatrix(), Resource.getMainCamera()->getViewMatrix());
    for (const auto& v : corners)
    {
        center = center + Vector3{v};
    }
    center = center / static_cast<float>(corners.size());

    const auto lightView = Matrix4::lookAt(center + m_direction, center, Vector3{0.0f, 1.0f, 0.0f});


    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    constexpr float zMult = 10.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    const auto lightProjection = Matrix4::makeOrthographicMatrix(minX, maxX, minY, maxY, minZ, maxZ);

    return lightProjection * lightView;
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
