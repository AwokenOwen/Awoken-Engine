//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Renderer.h"
#include "ResourceManager.h"

struct Mesh;
class ModelRendererComponent : public Component, public Renderer{
    friend class ResourceManager;
public:
    explicit ModelRendererComponent(Object* parent);
    ~ModelRendererComponent() override = default;

    void setActiveState(bool active) override;

private:
    void start() override;
    void update() override;

    void enable() override;
    void disable() override;

    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void draw() override;

public:
    void load() override;
    void unload() override;

private:
    std::string m_modelName{"assets/defaultAssets/Models/cube.fbx"};
    std::vector<std::string> m_materialNames{"assets/defaultAssets/Materials/unlit.json"};

    Model m_model{};
    std::vector<Material> m_materials{};
};
