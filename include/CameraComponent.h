//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "LinearMath.h"
#include "Renderer.h"
#include "ResourceManager.h"

#define PERSPECTIVE true
#define ORTHOGRAPHIC false

struct Texture;

class CameraComponent : public Component, public Renderer{
    friend class ResourceManager;
public:
    explicit CameraComponent(Object3D* parent);
    ~CameraComponent() override = default;

    void setActiveState(bool active) override;

    [[nodiscard]] Matrix4 getViewMatrix() const;
    [[nodiscard]] Matrix4 getPerspectiveMatrix() const;
    [[nodiscard]] Matrix4 getOrthographicMatrix() const;
    [[nodiscard]] Matrix4 getProjectionMatrix() const;
private:
    float m_fov{toRadians(90.0f)};
    float m_near{0.001f};
    float m_far{1000.0f};

    bool m_main{true};
    bool m_perspective{true};

    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;

    void start() override;
    void update() override;
    void enable() override;
    void disable() override;

    void draw() override;
    void drawToShadowMap(LightComponent* light) override;
    void destroy() override;

    std::string m_skyboxTextureName{"assets/defaultAssets/Skybox/skybox.hdr"};
    Texture m_skyboxTexture{};
    std::string m_skyboxModelName{"assets/defaultAssets/Models/cube.fbx"};
    Model m_skyboxModel{};
    std::string m_skyboxMaterialName{"assets/defaultAssets/Materials/skybox.json"};
    Material m_skyboxMaterial{};
};
