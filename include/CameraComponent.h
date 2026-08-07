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
    explicit CameraComponent(Object* parent);
    ~CameraComponent() override = default;

    void setActiveState(bool active) override;

    [[nodiscard]] Matrix4 getViewMatrix() const;
    [[nodiscard]] Matrix4 getPerspectiveMatrix() const;
    [[nodiscard]] Matrix4 getOrthographicMatrix() const;
    [[nodiscard]] Matrix4 getProjectionMatrix() const;

    void resetFramebuffers();

private:
    float m_fov{toRadians(90.0f)};
    float m_near{0.001f};
    float m_far{1000.0f};

    bool m_main{true};
    bool m_perspective{true};

    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;

    void drawToFrameBuffer();

    void createFrameBuffers();

    void start() override;
    void update() override;
    void enable() override;
    void disable() override;

    void draw();
    void drawToShadowMap(LightComponent* light);
    void destroy() override;

    /**
     * @brief Add lambda function to the draw event
     *
     * @param func Lambda function to be added raw to the draw event
     * @param transparent Whether the draw call should be in the transparent group or opaque group
     */
    void addToDraw(const std::function<void()>& func, bool transparent);

    std::string m_skyboxTextureName{"assets/defaultAssets/Skybox/skybox.hdr"};
    Texture m_skyboxTexture{};
    std::string m_skyboxModelName{"assets/defaultAssets/Models/cube.fbx"};
    Model m_skyboxModel{};
    std::string m_skyboxMaterialName{"assets/defaultAssets/Materials/skybox.json"};
    Material m_skyboxMaterial{};

    Event<> OpaqueDrawEvent{};
    Event<> TransparentDrawEvent{};
    Event<> ShadowDrawEvent{};

    FrameBuffer m_frameBuffer{};
    FrameBuffer m_intermediateFrameBuffer{};
};
