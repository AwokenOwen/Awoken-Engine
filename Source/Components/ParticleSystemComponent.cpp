//
// Created by awokenowen on 7/28/26.
//

#include "ParticleSystemComponent.h"

#include "CameraComponent.h"
#include "GameManager.h"
#include "Object.h"
#include <random>

void ParticleSystemComponent::setMaterial(const std::string& materialPath) {

    // Unload the current material
    Resource.unloadMaterial(m_materialPath);
    // Set the path of the new material
    m_materialPath = materialPath;

    // Load the new material
    Resource.loadMaterial(m_materialPath);
    // Set the material
    m_material = Resource.getMaterial(m_materialPath);
}

void ParticleSystemComponent::update() 
{
    // Add to draw every frame
    Resource.addToDraw(this, &ParticleSystemComponent::draw, false);
}

void ParticleSystemComponent::destroy() 
{
    // Delete the GPU buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

nlohmann::json ParticleSystemComponent::toJson() {
    // Create a JSON object
    nlohmann::json j;

    // Push the type
    j["Type"] = "ParticleSystem";

    // Push the material path
    j["Material"] = m_materialPath;

    return j;
}

void ParticleSystemComponent::fromJson(nlohmann::json j)
{
    // Create the buffers
    init();

    // Set the material path
    m_materialPath = j["Material"];
    // Load the material
    Resource.loadMaterial(m_materialPath);
    // Get the material
    m_material = Resource.getMaterial(m_materialPath);
}

void ParticleSystemComponent::draw() {
    // Turn on blend for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cull back face
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Load the material
    m_material.load();

    // Set the Matrix uniforms
    m_material.setUniform("model", getParent()->getWorldMatrix());
    m_material.setUniform("view", World.getMainCamera()->getViewMatrix());
    m_material.setUniform("projection", World.getMainCamera()->getProjectionMatrix());

    // Update the point buffer
    updatePoints();

    // Bind the vertex array
    glBindVertexArray(VAO);
    // Draw the particles
    glDrawArrays(GL_POINTS, 0, particles.numParticles);
    // Unbind
    glBindVertexArray(0);

}

void ParticleSystemComponent::drawToShadowMap(LightComponent *light) {
    // Nothing for now
}

void ParticleSystemComponent::init() {
    // Create the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the vertex array
    glBindVertexArray(VAO);

    // Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Create the size to be max particles and STREAM_DRAW for updating every frame
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Vector3), nullptr, GL_STREAM_DRAW);

    // Enable the vertex attribute
    glEnableVertexAttribArray(0);
    // Set the first attribute to be the position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), static_cast<void*>(nullptr));

    // Unbind
    glBindVertexArray(0);
}

void ParticleSystemComponent::updatePoints() {
    // Bind vertex array
    glBindVertexArray(VAO);

    // Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Update the data of the in the buffer to the correct data
    glBufferSubData( GL_ARRAY_BUFFER, 0, particles.numParticles * sizeof(Vector3), particles.positions.data());

    // Unbind
    glEnableVertexAttribArray(0);
}

void ParticleSystemComponent::addParticle() {
    // Nothing for now
}

void ParticleSystemComponent::removeParticle(const int i) {
    // Nothing for now
}
