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

Matrix4 CameraComponent::getPerspectiveMatrix()const {
    return {
        cot(m_fov / 2.0f) / Window.getAspectRatio(), 0, 0, 0,
        0, cot(m_fov / 2.0f), 0, 0,
        0, 0, (m_near + m_far) / (m_near - m_far), (2 * m_near * m_far) / (m_near - m_far),
        0, 0, -1, 0
    };
}

Matrix4 CameraComponent::getOrthographicMatrix() const
{
    float right = Window.getViewportWidth() / 2.0f;
    float left = -Window.getViewportWidth() / 2.0f;
    float top = Window.getViewportHeight() / 2.0f;
    float bottom = -Window.getViewportHeight() / 2.0f;
    return {
        2.0f / (right - left),  0.0f,                   0.0f,                  -(right + left) / (right - left),
        0.0f,                   2.0f / (top - bottom),  0.0f,                  -(top + bottom) / (top - bottom),
        0.0f,                   0.0f,                  -2.0f / (m_far - m_near),-(m_far + m_near)   / (m_far - m_near),
        0.0f,                   0.0f,                   0.0f,                   1.0f
    };
}

Matrix4 CameraComponent::makePerspectiveMatrix(float fov, float aspect, float near, float far)
{
    return {
        cot(fov / 2.0f) / aspect, 0, 0, 0,
        0, cot(fov / 2.0f), 0, 0,
        0, 0, (near + far) / (near - far), (2 * near * far) / (near - far),
        0, 0, -1, 0
    };
}

Matrix4 CameraComponent::makeOrthographicMatrix(float left, float right, float bottom, float top, float near, float far)
{
    return {
        2.0f / (right - left),  0.0f,                   0.0f,                  -(right + left) / (right - left),
        0.0f,                   2.0f / (top - bottom),  0.0f,                  -(top + bottom) / (top - bottom),
        0.0f,                   0.0f,                  -2.0f / (far - near),   -(far + near)   / (far - near),
        0.0f,                   0.0f,                   0.0f,                   1.0f
    };
}

void CameraComponent::load()
{
    m_skyboxTexture = Resource.getTexture(m_skyboxTextureName);

    Resource.loadModel(m_skyboxModelName);
    m_skyboxModel = Resource.getModel(m_skyboxModelName);

    Resource.loadMaterial(m_skyboxMaterialName);
    m_skyboxMaterial = Resource.getMaterial(m_skyboxMaterialName);
}

void CameraComponent::unload()
{

}

void CameraComponent::setBackgroundType(BackgroundType type)
{
    m_currentBackgroundType = type;
}

void CameraComponent::draw()
{
    if (m_currentBackgroundType == BackgroundType::SKYBOX)
    {
        m_skyboxMaterial.setUniform("view", getViewMatrix());
        m_skyboxMaterial.setUniform("projection", getPerspectiveMatrix());
        m_skyboxMaterial.load();

        m_skyboxMaterial.setUniform<int>("skybox", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.m_textureID);

        glBindVertexArray(m_skyboxModel.m_meshes[0].VAO());
        glDrawElements(GL_TRIANGLES, m_skyboxModel.m_meshes[0].indexCount(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }else
    {
        glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0);
    }

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

    j["FOV"] = m_fov;
    j["Near"] = m_near;
    j["Far"] = m_far;

    j["Main"] = m_main;

    j["BackgroundColor"] = m_backgroundColor.toJson();

    j["SkyboxTexture"] = m_skyboxTextureName;

    j["SkyboxModel"] = m_skyboxModelName;

    j["SkyboxMaterial"] = m_skyboxMaterialName;

    return j;
}

void CameraComponent::fromJson(nlohmann::json j)
{
    m_fov = j["FOV"].get<float>();
    m_near = j["Near"].get<float>();
    m_far = j["Far"].get<float>();

    m_main = j["Main"].get<bool>();
    if (m_main)
    {
        Resource.setMainCamera(this);
    }

    m_backgroundColor = Vector3::fromJson(j["BackgroundColor"]);

    m_skyboxTextureName = j["SkyboxTexture"].get<std::string>();

    m_skyboxModelName = j["SkyboxModel"].get<std::string>();

    m_skyboxMaterialName = j["SkyboxMaterial"].get<std::string>();

    registerRenderer(false);
}
