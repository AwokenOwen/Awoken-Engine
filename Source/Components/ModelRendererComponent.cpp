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
    glDisable(GL_CULL_FACE);
    if (m_materials.size() != m_model.meshCount())
    {
        Log.logError("Material Count does not match Mesh Count.");
        return;
    }
    for (int i = 0; i < m_model.meshCount(); ++i)
    {
        auto model = getParent()->getWorldMatrix();
        auto view = Resource.getMainCamera()->getViewMatrix();
        auto proj = Resource.getMainCamera()->getProjectionMatrix();

        m_materials[i].setUniform("model", model);
        m_materials[i].setUniform("view", view);
        m_materials[i].setUniform("projection", proj);

        m_materials[i].load();

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