//
// Created by AwokenOwen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "LinearMath.h"
#include "ResourceManager.h"

#define PERSPECTIVE true
#define ORTHOGRAPHIC false

struct Texture;

class CameraComponent : public Component {
    friend class ResourceManager;
    friend class WorldManager;
    friend struct Scene;
public:
    /**
     * @brief The constructor to set the parent object
     *
     * @param parent The object the component is attached to
     */
    explicit CameraComponent(Object* parent) : Component(parent){}

    /**
     * @brief Get the View Matrix of this Camera Component
     *
     * @return The view matrix of the camera
     */
    [[nodiscard]] Matrix4 getViewMatrix() const;
    /**
     * @brief Get the Perspective Matrix of this camera
     *
     * @return The perspective matrix of the camera
     */
    [[nodiscard]] Matrix4 getPerspectiveMatrix() const;
    /**
     * @brief Get the Orthographic Matrix of this camera
     *
     * @return The orthographic matrix of the camera
     */
    [[nodiscard]] Matrix4 getOrthographicMatrix() const;
    /**
     * @brief Returns Perspective or Orthographic Matrix based on camera settings
     *
     * @return The projection matrix of the camera
     */
    [[nodiscard]] Matrix4 getProjectionMatrix() const;

    /**
     * @brief Deletes both framebuffers and texture and remakes them, used when the viewport size gets updated for Post Processing and Anti Aliasing
     */
    void resetFramebuffers();

private:
    /**
     * @brief Activate the draw calls loaded into this camera
     */
    void drawToFrameBuffer();

    /**
     * @brief Helper function to create the framebuffers for the camera and store them for use
     */
    void createFrameBuffers();

    /**
     * @brief Function that runs only once when initialized
     */
    void start() override;
    /**
     * @brief Function that gets called once per frame it is active
     */
    void update() override;
    /**
     * @brief Function that gets called when the parent object or this component is set to be active
     */
    void enable() override{}
    /**
     * @brief Function that gets called when the parent object or this component is set to be inactive
     */
    void disable() override{}
    /**
     * @brief Function that gets called when the component is getting destroyed
     */
    void destroy() override;

    /**
     * @brief Function that takes the component and turns it into a JSON object
     *
     * @return JSON object will all the data for a camera component
     */
    nlohmann::json toJson() override;
    /**
     * @brief Function that takes a JSON object with all the data and builds the component
     *
     * @param j JSON object used to load the data of the camera component
     */
    void fromJson(nlohmann::json j) override;

    /**
     * @brief Function that draws the Skybox on the screen
     */
    void draw();
    /**
     * @brief Function that draws the onto a lights shadow texture
     *
     * @param light The light that holds the shadow texture
     */
    void drawToShadowMap(LightComponent* light);

    /**
     * @brief Add lambda function to the draw event
     *
     * @param func Lambda function to be added raw to the draw event
     * @param transparent Whether the draw call should be in the transparent group or opaque group
     */
    void addToDraw(const std::function<void()>& func, bool transparent);

    /**
     * @brief Field of view of the camera
     */
    float m_fov{toRadians(90.0f)};
    /**
     * @brief The near range of the camera
     */
    float m_near{0.001f};
    /**
     * @brief The far range of the camera
     */
    float m_far{1000.0f};

    /**
     * @brief Whether this camera is the main camera that writes to the screen
     */
    bool m_main{true};
    /**
     * @brief Whether this camera is a Perspective Camera or an Orthographic camera
     */
    bool m_perspective{true};

    /**
     * @brief The Path to the skybox texture
     */
    std::string m_skyboxTexturePath{"assets/defaultAssets/Skybox/skybox.hdr"};
    /**
     * @brief The loaded texture data
     */
    Texture m_skyboxTexture{};
    /**
     * @brief The Path to the skybox model (pretty much always a cube)
     */
    std::string m_skyboxModelPath{"assets/defaultAssets/Models/cube.fbx"};
    /**
     * @brief The loaded skybox model
     */
    Model m_skyboxModel{};
    /**
     * @brief The Path to the skybox material
     */
    std::string m_skyboxMaterialPath{"assets/defaultAssets/Materials/skybox.json"};
    /**
     * @brief The loaded material
     */
    Material m_skyboxMaterial{};

    /**
     * @brief The draw event for all opaque draw calls
     */
    Event<> OpaqueDrawEvent{};
    /**
     * @brief The draw event for all transparent draw calls
     */
    Event<> TransparentDrawEvent{};

    /**
     * @brief The main framebuffer objects get draw to for anti aliasing
     */
    FrameBuffer m_frameBuffer{};
    /**
     * @brief The intermediate framebuffer that reads the main framebuffer to a texture
     */
    FrameBuffer m_intermediateFrameBuffer{};
};
