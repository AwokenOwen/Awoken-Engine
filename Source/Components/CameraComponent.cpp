//
// Created by awokenowen on 4/24/26.
//

#include "CameraComponent.h"

#include "Object.h"
#include "ResourceManager.h"
#include "WindowManager.h"

Matrix4 CameraComponent::getViewMatrix() const {
    // Get the parent position
    const Vector3 pos     = getParent()->getWorldPosition();
    // Get the parent Forward Vector normalized
    const Vector3 forward = getParent()->getWorldForward().normalize();

    // Return the look at function toward where the Camera is looking
    return Matrix4::LookAt(pos, pos + forward, Vector3::up());
}

Matrix4 CameraComponent::getPerspectiveMatrix()const {
    // Return the Perspective Matrix relative to this Camera
    return Matrix4::PerspectiveMatrix(m_fov, Window.getAspectRatio(), m_near, m_far);
}

Matrix4 CameraComponent::getOrthographicMatrix() const
{
    // Get the centered positions of the right, left, top, and bottom walls of the viewport
    const float right = Window.getViewportWidth() / 2.0f;
    const float left = -Window.getViewportWidth() / 2.0f;
    const float top = Window.getViewportHeight() / 2.0f;
    const float bottom = -Window.getViewportHeight() / 2.0f;

    // Return the Orthographic Matrix relative to this Camera
    return Matrix4::OrthographicMatrix(left, right, bottom, top, m_near, m_far);
}

Matrix4 CameraComponent::getProjectionMatrix() const
{
    // Get the Projection Matrix relative to whether the Camera is a Perspective Camera or not
    return m_perspective ? getPerspectiveMatrix() : getOrthographicMatrix();
}

void CameraComponent::resetFramebuffers()
{
    // Delete the main frame buffer
    glDeleteFramebuffers(1, &m_frameBuffer.m_id);
    // Delete the main framebuffer's texture
    glDeleteTextures(1, &m_frameBuffer.m_colorBuffer);

    // Delete the intermediate frame buffer
    glDeleteFramebuffers(1, &m_intermediateFrameBuffer.m_id);
    // Delete the intermediate frame buffer's texture
    glDeleteTextures(1, &m_intermediateFrameBuffer.m_colorBuffer);

    // Create new frame buffers to match the viewport
    createFrameBuffers();
}

void CameraComponent::addToDraw(const std::function<void()>& func, const bool transparent)
{
    // Add the function to the correct event
    transparent ? TransparentDrawEvent.addRaw(func) : OpaqueDrawEvent.addRaw(func);
}

void CameraComponent::draw()
{
    // Enable CULL_FACE
    glEnable(GL_CULL_FACE);
    // Cull the front face (because this is a skybox)
    glCullFace(GL_FRONT);

    // Get a the view Matrix and remove the transpose element (by shrinking the Matrix to 3x3 then enlarging it with 0s back to 4x4)
    const auto view = Matrix4(Matrix3(getViewMatrix()));
    // Load the Material
    m_skyboxMaterial.load();
    // Set the Matrices in the Material
    m_skyboxMaterial.setUniform("view", view);
    m_skyboxMaterial.setUniform("projection", getPerspectiveMatrix());

    // Set the texture to the texture saved in the class
    m_skyboxMaterial.setUniform<int>("skybox", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.m_textureID);

    // Draw the Skybox
    glBindVertexArray(m_skyboxModel.m_meshes[0].VAO());
    glDrawElements(GL_TRIANGLES, m_skyboxModel.m_meshes[0].indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // Disable cull face
    glDisable(GL_CULL_FACE);
}

void CameraComponent::drawToShadowMap(LightComponent* light)
{
    // Finish later
}

void CameraComponent::start()
{
    // Add this camera to the scene's list of cameras
    getParent()->getScene()->addCamera(this);

    // Load and get skybox texture
    Resource.loadTexture(m_skyboxTexturePath);
    m_skyboxTexture = Resource.getTexture(m_skyboxTexturePath);

    // Load and get skybox model
    Resource.loadModel(m_skyboxModelPath);
    m_skyboxModel = Resource.getModel(m_skyboxModelPath);

    // Load and get skybox material
    Resource.loadMaterial(m_skyboxMaterialPath);
    m_skyboxMaterial = Resource.getMaterial(m_skyboxMaterialPath);

    // Create the framebuffers
    createFrameBuffers();
}

void CameraComponent::update()
{
    // Add the skybox to the list of draw calls on this camera only
    addToDraw([this](){draw();}, false);
}

void CameraComponent::destroy()
{
    // Delete the main frame buffer
    glDeleteFramebuffers(1, &m_frameBuffer.m_id);
    // Delete the main framebuffer's texture
    glDeleteTextures(1, &m_frameBuffer.m_colorBuffer);

    // Delete the intermediate frame buffer
    glDeleteFramebuffers(1, &m_intermediateFrameBuffer.m_id);
    // Delete the intermediate frame buffer's texture
    glDeleteTextures(1, &m_intermediateFrameBuffer.m_colorBuffer);
}

nlohmann::json CameraComponent::toJson()
{
    // Create a JSON object
    nlohmann::json j;

    // Push the type to "Camera"
    j["Type"] = "Camera";

    // Push the FOV
    j["FOV"] = m_fov;
    // Push the near limit
    j["Near"] = m_near;
    // Push the far limit
    j["Far"] = m_far;

    // Push if this camera is the main camera
    j["Main"] = m_main;

    // Push the skybox texture path
    j["SkyboxTexture"] = m_skyboxTexturePath;
    // Push the skybox model path
    j["SkyboxModel"] = m_skyboxModelPath;
    // Push the skybox material path
    j["SkyboxMaterial"] = m_skyboxMaterialPath;

    // Push the camera projection type
    j["Perspective"] = m_perspective;

    return j;
}

void CameraComponent::fromJson(nlohmann::json j)
{
    // Set the FOV
    m_fov = j["FOV"].get<float>();
    // Set the near limit
    m_near = j["Near"].get<float>();
    // Set the far limit
    m_far = j["Far"].get<float>();

    // Set if this camera is the main camera
    m_main = j["Main"].get<bool>();
    // If so set it in the world
    if (m_main)
    {
        // Set this as main camera
        World.setMainCamera(this);
    }

    // Set the skybox texture path
    m_skyboxTexturePath = j["SkyboxTexture"].get<std::string>();
    // Set the skybox model path
    m_skyboxModelPath = j["SkyboxModel"].get<std::string>();
    // Set the skybox material path
    m_skyboxMaterialPath = j["SkyboxMaterial"].get<std::string>();

    // Set camera projection type
    m_perspective = j["Perspective"].get<bool>();
}

void CameraComponent::drawToFrameBuffer()
{
    // Bind the main framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.m_id);
    // Clear the buffer bits
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Draw all objects queued to be drawn this frame
    OpaqueDrawEvent.call();
    TransparentDrawEvent.call();

    // Clear all draw calls
    OpaqueDrawEvent.clear();
    TransparentDrawEvent.clear();

    // Draw the resulted Anti Aliasing draw to a normal texture to be rendered on the screen in some way
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer.m_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediateFrameBuffer.m_id);
    glBlitFramebuffer(0, 0, Window.getViewportWidth(), Window.getViewportHeight(), 0, 0, Window.getViewportWidth(), Window.getViewportHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Reactivate the normal screen framebuffer
    Resource.activateFramebuffer();
}

void CameraComponent::createFrameBuffers()
{
    // Grab the current viewport size
    const auto width = Window.getViewportWidth();
    const auto height = Window.getViewportHeight();

    // Create main framebuffer
    glGenFramebuffers(1, &m_frameBuffer.m_id);
    // Bind the newly created buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.m_id);
    // Create the texture for the framebuffer
    glGenTextures(1, &m_frameBuffer.m_colorBuffer);
    // Bind the Texture as a TEXTURE 2D MULTISAMPLE
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_frameBuffer.m_colorBuffer);
    // Create the data to be that of a TEXTURE 2D MULTISAMPLE with 4 samples (you don't really need more than that) RGB and correct width and height
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
    // Bind no texture
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    // Set the texture to be the color attachment of the main framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_frameBuffer.m_colorBuffer, 0);

    // Create a renderbuffer for depth and stencil attachment for the main framebuffer
    glGenRenderbuffers(1, &m_frameBuffer.m_renderBuffer);
    // Bind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_frameBuffer.m_renderBuffer);
    // Set the storage to multisample with 4 samples as well and have it capture depth and stencil data
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    // Bind no renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Set the render buffer to be the depth and stencil attachment of the main framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_frameBuffer.m_renderBuffer);

    // Error test the creation of the main framebuffer
    if (const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Main framebuffer for camera not complete: %d", fboStatus);
    // Bind no framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create intermediate framebuffer
    glGenFramebuffers(1, &m_intermediateFrameBuffer.m_id);
    // Bind to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_intermediateFrameBuffer.m_id);

    // Create the texture for the framebuffer
    glGenTextures(1, &m_intermediateFrameBuffer.m_colorBuffer);
    // Bind it as a normal texture 2D
    glBindTexture(GL_TEXTURE_2D, m_intermediateFrameBuffer.m_colorBuffer);
    // Create the data to be a normal Texture 2D RGB and correct viewport size
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    // Set the min and max filter to Linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Bind no texture
    glBindTexture(GL_TEXTURE_2D, 0);
    /// Set the texture to be the color attachment of the intermediate framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_intermediateFrameBuffer.m_colorBuffer, 0);

    // Error checking the intermediate Framebuffer
    if (const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Framebuffer not complete: %d", fboStatus);
    // Bind no framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
