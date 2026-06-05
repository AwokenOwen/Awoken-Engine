//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Math.h"
#include "Renderer.h"
#include "ResourceManager.h"

#define PERSPECTIVE true
#define ORTHOGRAPHIC false

struct Texture;

enum BackgroundType
{
    SOLID = 0,
    SKYBOX = 1
};

class CameraComponent : public Component, public Renderer{
    friend class ResourceManager;
public:
    explicit CameraComponent(Object* parent);
    ~CameraComponent() override = default;

    void setActiveState(bool active) override;

    [[nodiscard]] Matrix4 getViewMatrix() const;
    [[nodiscard]] Matrix4 getPerspectiveMatrix() const;
    [[nodiscard]] Matrix4 getOrthographicMatrix() const;
    [[nodiscard]] static Matrix4 makePerspectiveMatrix(float fov, float aspect, float near, float far);
    [[nodiscard]] static Matrix4 makeOrthographicMatrix(float left, float right, float bottom, float top, float near, float far);

    void load() override;
    void unload() override;

    void setBackgroundType(BackgroundType type);

    bool m_main{false};

private:
    float m_fov{toRadians(90.0f)};
    float m_near{0.001f};
    float m_far{1000.0f};

    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;

    void start() override;
    void update() override;
    void enable() override;
    void disable() override;

    void draw() override;

    BackgroundType m_currentBackgroundType{BackgroundType::SKYBOX};

    Vector3 m_backgroundColor{0.0f, 0.0f, 0.0f};
    std::string m_skyboxTextureName{"assets/defaultAssets/Skybox/skybox.hdr"};
    Texture m_skyboxTexture{};
    std::string m_skyboxModelName{"assets/defaultAssets/Models/cube.fbx"};
    Model m_skyboxModel{};
    std::string m_skyboxMaterialName{"assets/defaultAssets/Materials/skybox.json"};
    Material m_skyboxMaterial{};
};
