//
// Created by awokenowen on 7/28/26.
//
#pragma once
#include "Component.h"
#include "ResourceManager.h"

// Struct of arrays instead of array of structs
struct Particles {
    // All the positions of the particles
    std::vector<Vector3> positions {};
    // All the speeds of the particles
    std::vector<Vector3> velocities {};
    // All the lifetimes of the particles
    std::vector<float> lifeTimes {};
    // The number of current particles
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
