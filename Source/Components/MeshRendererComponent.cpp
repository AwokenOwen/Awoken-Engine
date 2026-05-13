//
// Created by awokenowen on 4/24/26.
//

#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(Object *parent) : Component(parent), Renderer(false) {

}

void MeshRendererComponent::setActiveState(const bool active) {
    Component::setActiveState(active);
    World.setActiveRenderer(this, active);
}

void MeshRendererComponent::start() {
}

void MeshRendererComponent::update() {
}

void MeshRendererComponent::draw() {
}

void MeshRendererComponent::enable()
{
}

void MeshRendererComponent::disable()
{
}

nlohmann::json MeshRendererComponent::toJson()
{
    return {};
}

void MeshRendererComponent::fromJson(nlohmann::json j)
{
}
