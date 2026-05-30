//
// Created by awokenowen on 4/24/26.
//

#include "ModelRendererComponent.h"

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

    return j;
}

void ModelRendererComponent::fromJson(nlohmann::json j)
{
    m_modelName = j["Model"].get<std::string>();

    registerRenderer(j["Transparency"].get<bool>());
}

void ModelRendererComponent::draw()
{

}

void ModelRendererComponent::load()
{
    Resource.loadModel(m_modelName);
    m_model = Resource.getModel(m_modelName);
}

void ModelRendererComponent::unload()
{

}