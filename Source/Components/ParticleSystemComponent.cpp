//
// Created by awokenowen on 7/28/26.
//

#include "ParticleSystemComponent.h"

#include "CameraComponent.h"
#include "GameManager.h"
#include "Object.h"
#include <random>

void ParticleSystemComponent::setMaterial(const std::string& materialPath) {

    Resource.unloadMaterial(materialPath);
    Resource.loadMaterial(materialPath);
    m_materialPath = materialPath;

    m_material = Resource.getMaterial(materialPath);
}

void ParticleSystemComponent::start() {

}

void ParticleSystemComponent::update() {

}

void ParticleSystemComponent::enable() {
}

void ParticleSystemComponent::disable() {
}

void ParticleSystemComponent::destroy() {
}

nlohmann::json ParticleSystemComponent::toJson() {
    nlohmann::json j;

    j["Type"] = "ParticleSystem";

    j["Material"] = m_materialPath;

    return j;
}

void ParticleSystemComponent::fromJson(nlohmann::json j)
{
    init();

    m_materialPath = j["Material"];
    Resource.loadMaterial(m_materialPath);

    m_material = Resource.getMaterial(m_materialPath);
}

void ParticleSystemComponent::draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    m_material.load();

    m_material.setUniform("model", getParent()->getWorldMatrix());
    m_material.setUniform("view", World.getMainCamera()->getViewMatrix());
    m_material.setUniform("projection", World.getMainCamera()->getProjectionMatrix());

    updatePoints();

    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, particles.numParticles);

    glBindVertexArray(0);

}

void ParticleSystemComponent::drawToShadowMap(LightComponent *light) {
}

void ParticleSystemComponent::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Vector3), nullptr, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), static_cast<void*>(nullptr));

    glBindVertexArray(0);
}

void ParticleSystemComponent::updatePoints() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData( GL_ARRAY_BUFFER, 0, particles.numParticles * sizeof(Vector3), particles.positions.data());

    glEnableVertexAttribArray(0);
}

void ParticleSystemComponent::addParticle() {

}

void ParticleSystemComponent::removeParticle(const int i) {

}
