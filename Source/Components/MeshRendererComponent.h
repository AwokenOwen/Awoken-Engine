//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Renderer.h"

class MeshRendererComponent : public Component, public Renderer{
public:
    explicit MeshRendererComponent(Object* parent);
    ~MeshRendererComponent() override = default;

    void setActiveState(bool active) override;

private:
    void start() override;
    void update() override;

    void draw() override;
};
