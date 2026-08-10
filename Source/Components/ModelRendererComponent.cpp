//
// Created by awokenowen on 4/24/26.
//

#include "ModelRendererComponent.h"

#include <iostream>

#include "CameraComponent.h"
#include "LightComponent.h"
#include "LogManager.h"
#include "Object.h"

ModelRendererComponent::ModelRendererComponent(Object *parent) : Component(parent) {

}

void ModelRendererComponent::setActiveState(const bool active) {
    Component::setActiveState(active);
}

void ModelRendererComponent::start() {
    
}

void ModelRendererComponent::update()
{
    // decide whether to draw
    m_model.m_boundingBox = m_model.m_boundingBox * getParent()->getWorldMatrix();

    if (m_model.meshCount() != m_materials.size())
        Log.logError("Mesh Count does not match Material Count.");
    for (int i = 0; i < m_model.meshCount(); ++i)
    {
        Resource.addToDraw([this, i]()
        {
            this->defaultDynamicUniformLoader(m_materials[i]);

            glBindVertexArray(m_model.m_meshes[i].VAO());
            glDrawElements(GL_TRIANGLES, m_model.m_meshes[i].indexCount(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }, false);
    }
}

void ModelRendererComponent::enable()
{
}

void ModelRendererComponent::disable()
{
}

nlohmann::json ModelRendererComponent::toJson()
{
    nlohmann::json j;

    j["Type"] = "ModelRenderer";

    j["Model"] = m_modelName;

    j["Materials"] = m_materialNames;

    return j;
}

void ModelRendererComponent::fromJson(nlohmann::json j)
{
    m_modelName = j["Model"].get<std::string>();

    m_materialNames = j["Materials"].get<std::vector<std::string>>();

    Resource.loadModel(m_modelName);
    m_model = Resource.getModel(m_modelName);

    for (const auto& m : m_materialNames)
    {
        Resource.loadMaterial(m);
        m_materials.emplace_back(Resource.getMaterial(m));
    }
    m_shadowMapMaterial = Resource.getMaterial("assets/defaultAssets/Materials/shadowMap.json");
}

void ModelRendererComponent::defaultDynamicUniformLoader(Material mat) const
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    mat.load();

    const auto model = getParent()->getWorldMatrix();
    const auto view = World.getMainCamera()->getViewMatrix();
    const auto proj = World.getMainCamera()->getProjectionMatrix();
    const auto orthographic = World.getMainCamera()->getOrthographicMatrix();

    switch (mat.getType())
    {
    case NONE:
        break;
    case UNLIT:
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        mat.setUniform("projection", proj);
        break;
    case DEFAULT_LIT:
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        mat.setUniform("projection", proj);

        // Also add lighting here
        Resource.loadLights(mat);
        break;
    case DEFAULT_SPRITE:
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        mat.setUniform("projection", orthographic);
        break;
    }

    glDisable(GL_CULL_FACE);
}

void ModelRendererComponent::destroy()
{
    Resource.unloadModel(m_modelName);
    m_model = {};

    for (const auto& m : m_materialNames)
    {
        Resource.unloadMaterial(m);
    }
    m_materials.clear();

    delete this;
}
