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

/**
* @brief Component for handling particle simulations
*/
class ParticleSystemComponent : public Component {
public:
    friend class ResourceManager;
    /**
     * @brief The constructor for the component to set parent of the component
     * 
     * @param parent The parent object of the component
     */
    explicit ParticleSystemComponent(Object *parent) : Component(parent) {}

    /**
     * @brief Function to change the material for the renderer
     * 
     * @param materialPath Path to the new material
     */
    void setMaterial(const std::string& materialPath);

private:
    /**
     * @brief Function that runs on the first frame it exists
     */
    void start() override {}
    /**
     * @brief Function that runs every frame it is active
     */
    void update() override;
    /**
     * @brief Function that runs when the component is set active
     */
    void enable() override {}
    /**
     * @brief Function that runs when the component is set inactive
     */
    void disable() override {}
    /**
     * @brief Function that runs when the component is being destroyed
     */
    void destroy() override;

    /**
     * @brief Function that takes all the component data and returns a JSON object
     * 
     * @return JSON object with all the data needed to run the component
     */
    nlohmann::json toJson() override;
    /**
     * @brief Function to build the component from a JSON object
     * 
     * @param j The JSON object with all the data needed to build the component
     */
    void fromJson(nlohmann::json j) override;

    /**
     * @brief Function for drawing the particles to the screen
     */
    void draw();

    /**
     * @brief Function for drawing shadows of the particles
     * 
     * @param light The light source for drawing purposes
     */
    void drawToShadowMap(LightComponent *light);

    /**
     * @brief Helper function called to initialized the Particle System buffers
     */
    void init();

    /**
     * @brief Helper function called to update the VBO of the particle locations
     */
    void updatePoints();

    /**
     * @brief Adds a particle to the list of active particles
     */
    void addParticle();
    /**
     * @brief Removes a particle from the list of active particles
     * 
     * @param i The index of the active particle
     */
    void removeParticle(int i);

    /**
     * @brief The Buffers for the particle system
     */
    unsigned int VAO{}, VBO{};

    /**
     * @brief The material for the particles
     */
    std::string m_materialPath{"assets/defaultAssets/Materials/particle.json"};
    /**
     * @brief The loaded material
     */
    Material m_material{};

    /**
     * @brief the max number of particles allows to exist
     */
    int maxParticles{100};

    /**
     * @brief The time the particle system has exisited
     */
    float timer{0.0f};

    /**
     * @brief Struct of arrays of the particles
     */
    Particles particles;
};