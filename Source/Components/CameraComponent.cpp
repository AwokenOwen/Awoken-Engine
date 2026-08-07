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

    return Matrix4::LookAt(pos, pos + forward, Vector3::up());
}

Matrix4 CameraComponent::getPerspectiveMatrix()const {
    return Matrix4::PerspectiveMatrix(m_fov, Window.getAspectRatio(), m_near, m_far);
}

Matrix4 CameraComponent::getOrthographicMatrix() const
{
    float right = Window.getViewportWidth() / 2.0f;
    float left = -Window.getViewportWidth() / 2.0f;
    float top = Window.getViewportHeight() / 2.0f;
    float bottom = -Window.getViewportHeight() / 2.0f;

    return Matrix4::OrthographicMatrix(left, right, bottom, top, m_near, m_far);
}

Matrix4 CameraComponent::getProjectionMatrix() const
{
    if (m_perspective)
    {
        return getPerspectiveMatrix();
    }
    return getOrthographicMatrix();
}

void CameraComponent::resetFramebuffers()
{
    glDeleteFramebuffers(1, &m_frameBuffer.m_id);
    glDeleteTextures(1, &m_frameBuffer.m_colorBuffer);

    glDeleteFramebuffers(1, &m_intermediateFrameBuffer.m_id);
    glDeleteTextures(1, &m_intermediateFrameBuffer.m_colorBuffer);

    createFrameBuffers();
}

void CameraComponent::addToDraw(const std::function<void()>& func, const bool transparent)
{
    transparent ? TransparentDrawEvent.addRaw(func) : OpaqueDrawEvent.addRaw(func);
}

void CameraComponent::draw()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    const auto view = Matrix3(getViewMatrix());
    m_skyboxMaterial.load();
    m_skyboxMaterial.setUniform("view", Matrix4(view));
    m_skyboxMaterial.setUniform("projection", getPerspectiveMatrix());

    m_skyboxMaterial.setUniform<int>("skybox", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.m_textureID);

    glBindVertexArray(m_skyboxModel.m_meshes[0].VAO());
    glDrawElements(GL_TRIANGLES, m_skyboxModel.m_meshes[0].indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);
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
    getParent()->getScene()->addCamera(this);
}

void CameraComponent::update()
{
    Resource.addToDraw(this, &CameraComponent::draw, false);
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
        World.setMainCamera(this);
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

    createFrameBuffers();
}

void CameraComponent::drawToFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.m_id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    OpaqueDrawEvent.call();
    TransparentDrawEvent.call();

    OpaqueDrawEvent.clear();
    TransparentDrawEvent.clear();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer.m_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediateFrameBuffer.m_id);
    glBlitFramebuffer(0, 0, Window.getViewportWidth(), Window.getViewportHeight(), 0, 0, Window.getViewportWidth(), Window.getViewportHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    Resource.activateFramebuffer();
}

void CameraComponent::createFrameBuffers()
{
    const auto width = Window.getViewportWidth();
    const auto height = Window.getViewportHeight();

    glGenFramebuffers(1, &m_frameBuffer.m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.m_id);
    // create a color attachment texture
    glGenTextures(1, &m_frameBuffer.m_colorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_frameBuffer.m_colorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_frameBuffer.m_colorBuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &m_frameBuffer.m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_frameBuffer.m_renderBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_frameBuffer.m_renderBuffer);

    if (const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Framebuffer not complete: %d", fboStatus);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_intermediateFrameBuffer.m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_intermediateFrameBuffer.m_id);

    glGenTextures(1, &m_intermediateFrameBuffer.m_colorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_intermediateFrameBuffer.m_colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_intermediateFrameBuffer.m_colorBuffer, 0);

    if (const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Framebuffer not complete: %d", fboStatus);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
