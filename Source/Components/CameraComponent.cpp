//
// Created by awokenowen on 4/24/26.
//

#include "CameraComponent.h"

#include "Object.h"
#include "ResourceManager.h"
#include "WindowManager.h"

CameraComponent::CameraComponent(Object *parent) : Component(parent) {
}

Matrix4 CameraComponent::getViewMatrix() const {
    if (getParent() == nullptr) return Matrix4{};

    const Vector3 pos     = getParent()->getWorldPosition();
    const Vector3 forward = getParent()->getWorldForward();
    const Vector3 up      = getParent()->getWorldUp();
    const Vector3 f       = (forward).normalize();
    const Vector3 r       = Vector3::cross(f, up).normalize();
    const Vector3 u       = Vector3::cross(r, f);

    return {
         r.x,  r.y,  r.z, -Vector3::dot(r, pos),
         u.x,  u.y,  u.z, -Vector3::dot(u, pos),
        -f.x, -f.y, -f.z,  Vector3::dot(f, pos),
         0,    0,    0,    1
    };
}

Matrix4 CameraComponent::getProjectionMatrix() const {
    if (m_projectionType) {
        return {
            cot(m_fov / 2.0f) / Window.getAspectRatio(), 0, 0, 0,
            0, cot(m_fov / 2.0f), 0, 0,
            0, 0, (m_near + m_far) / (m_near - m_far), (2 * m_near * m_far) / (m_near - m_far),
            0, 0, -1, 0
        };
    }
    float r = Window.getAspectRatio();
    float t = 1.0f;
    return {
        1.0f/r, 0, 0, 0,
        0, 1.0f/t, 0, 0,
        0, 0, -2.0f/(m_far - m_near), -(m_far + m_near)/(m_far - m_near),
        0, 0, 0, 1
    };
}

void CameraComponent::setProjectionType(const bool projectionType) {
    m_projectionType = projectionType;
}

void CameraComponent::start()
{

}

void CameraComponent::update()
{

}

void CameraComponent::enable()
{

}

void CameraComponent::disable()
{

}

void CameraComponent::setActiveState(const bool active)
{
    Component::setActiveState(active);
}

nlohmann::json CameraComponent::toJson()
{
    nlohmann::json j;

    j["Type"] = "Camera";

    j["ProjectionType"] = m_projectionType;
    j["FOV"] = m_fov;
    j["Near"] = m_near;
    j["Far"] = m_far;

    j["Main"] = m_main;

    return j;
}

void CameraComponent::fromJson(nlohmann::json j)
{
    m_projectionType = j["ProjectionType"].get<bool>();
    m_fov = j["FOV"].get<float>();
    m_near = j["Near"].get<float>();
    m_far = j["Far"].get<float>();

    m_main = j["Main"].get<bool>();
    if (m_main)
    {
        Resource.setMainCamera(this);
    }
}
