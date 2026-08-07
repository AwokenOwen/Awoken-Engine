//
// Created by awokenowen on 7/28/26.
//
#pragma once
#include "Component.h"
#include "ResourceManager.h"

struct Particles {
    std::vector<Vector3> positions {};
    std::vector<Vector3> velocities {};
    std::vector<float> lifeTimes {};
    int numParticles{};
};

class ParticleSystemComponent : public Component {
public:
    friend class ResourceManager;
    explicit ParticleSystemComponent(Object *parent)
        : Component(parent) {
    }

    void setMaterial(const std::string& materialPath);

private:
    void start() override;

    void update() override;

    void enable() override;

    void disable() override;

    void destroy() override;

    nlohmann::json toJson() override;

    void fromJson(nlohmann::json j) override;

    void draw();

    void drawToShadowMap(LightComponent *light);

    void init();

    void updatePoints();

    void addParticle();
    void removeParticle(int i);

    unsigned int VAO{}, VBO{};

    std::string m_materialPath{"assets/defaultAssets/Materials/particle.json"};
    Material m_material{};

    int maxParticles{100};

    float timer{0.0f};

    Particles particles;
};
