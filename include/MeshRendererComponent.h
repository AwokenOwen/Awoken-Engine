//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Renderer.h"

class MeshRendererComponent : public Component, public Renderer{
    friend class ResourceManager;
public:
    explicit MeshRendererComponent(Object* parent);
    ~MeshRendererComponent() override = default;

    void setActiveState(bool active) override;

private:
    void start() override;
    void update() override;

    void draw() override;
    void enable() override;
    void disable() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
};
