//
// Created by awokenowen on 4/24/26.
//

#include "ModelRendererComponent.h"

#include "CameraComponent.h"
#include "LogManager.h"
#include "Object.h"

ModelRendererComponent::ModelRendererComponent(Object *parent) : Component(parent), Renderer() {

}

void ModelRendererComponent::setActiveState(const bool active) {
    Component::setActiveState(active);
}

void ModelRendererComponent::start() {
    
}

void ModelRendererComponent::update() {

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

    j["Transparency"] = getTransparency();

    j["Materials"] = m_materialNames;

    return j;
}

void ModelRendererComponent::fromJson(nlohmann::json j)
{
    m_modelName = j["Model"].get<std::string>();

    registerRenderer(j["Transparency"].get<bool>());

    m_materialNames = j["Materials"].get<std::vector<std::string>>();
}

void ModelRendererComponent::draw()
{
    if (m_materials.size() != m_model.meshCount())
    {
        Log.logError("Material Count does not match Mesh Count.");
        return;
    }
    for (int i = 0; i < m_model.meshCount(); ++i)
    {
        defaultDynamicUniformLoader(m_materials[i]);

        glBindVertexArray(m_model.m_meshes[i].VAO());
        glDrawElements(GL_TRIANGLES, m_model.m_meshes[i].indexCount(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}

void ModelRendererComponent::load()
{
    Resource.loadModel(m_modelName);
    m_model = Resource.getModel(m_modelName);

    for (const auto& m : m_materialNames)
    {
        Resource.loadMaterial(m);
        m_materials.emplace_back(Resource.getMaterial(m));
    }
}

void ModelRendererComponent::unload()
{
    Resource.unloadModel(m_modelName);
    m_model = {};

    for (const auto& m : m_materialNames)
    {
        Resource.unloadMaterial(m);
    }
    m_materials.clear();
}

void ModelRendererComponent::defaultDynamicUniformLoader(Material mat) const
{
    glEnable(GL_CULL_FACE);
    mat.load();

    const auto model = getParent()->getWorldMatrix();
    const auto view = Resource.getMainCamera()->getViewMatrix();
    const auto proj = Resource.getMainCamera()->getProjectionMatrix();
    const auto orthographic = Resource.getMainCamera()->getOrthographicMatrix();

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

        break;
    case DEFAULT_SPRITE:
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        mat.setUniform("projection", orthographic);
        break;
    }
}
