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
    const Vector3 forward = getParent()->getWorldForward().normalize();
    const Vector3 up      = getParent()->getWorldUp();

    return Matrix4::lookAt(pos, pos + forward, up);
}

Matrix4 CameraComponent::getPerspectiveMatrix()const {
    return Matrix4::makePerspectiveMatrix(m_fov, Window.getAspectRatio(), m_near, m_far);
}

Matrix4 CameraComponent::getOrthographicMatrix() const
{
    float right = Window.getViewportWidth() / 2.0f;
    float left = -Window.getViewportWidth() / 2.0f;
    float top = Window.getViewportHeight() / 2.0f;
    float bottom = -Window.getViewportHeight() / 2.0f;

    return Matrix4::makeOrthographicMatrix(left, right, bottom, top, m_near, m_far);
}

Matrix4 CameraComponent::getProjectionMatrix() const
{
    if (m_perspective)
    {
        return getPerspectiveMatrix();
    }
    return getOrthographicMatrix();
}

void CameraComponent::draw()
{
    glDisable(GL_CULL_FACE);
    auto view = Matrix3(getViewMatrix());
    m_skyboxMaterial.setUniform("view", Matrix4(view));
    m_skyboxMaterial.setUniform("projection", getPerspectiveMatrix());
    m_skyboxMaterial.load();

    m_skyboxMaterial.setUniform<int>("skybox", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.m_textureID);

    glBindVertexArray(m_skyboxModel.m_meshes[0].VAO());
    glDrawElements(GL_TRIANGLES, m_skyboxModel.m_meshes[0].indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void CameraComponent::drawToShadowMap(LightComponent* light)
{

}

void CameraComponent::destroy()
{
    delete this;
}

void CameraComponent::start()
{

}

void CameraComponent::update()
{
    addToDraw();
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

    j["SkyboxTexture"] = m_skyboxTextureName;

    j["SkyboxModel"] = m_skyboxModelName;

    j["SkyboxMaterial"] = m_skyboxMaterialName;

    j["Perspective"] = m_perspective;

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

    m_skyboxTextureName = j["SkyboxTexture"].get<std::string>();

    m_skyboxModelName = j["SkyboxModel"].get<std::string>();

    m_skyboxMaterialName = j["SkyboxMaterial"].get<std::string>();

    m_perspective = j["Perspective"].get<bool>();

    Resource.loadTexture(m_skyboxTextureName);
    m_skyboxTexture = Resource.getTexture(m_skyboxTextureName);

    Resource.loadModel(m_skyboxModelName);
    m_skyboxModel = Resource.getModel(m_skyboxModelName);

    Resource.loadMaterial(m_skyboxMaterialName);
    m_skyboxMaterial = Resource.getMaterial(m_skyboxMaterialName);
}
