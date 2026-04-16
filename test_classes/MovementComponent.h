//
// Created by awokenowen on 3/29/26.
//
#pragma once
#include "../Component.h"
#include "../InputManager.h"

class MovementComponent : public Component{
public:
    explicit MovementComponent(Object* parent);

    void handleKeyboard(KeyboardContext context);
    void handleMouse(glm::vec2 pos, glm::vec2 delta);

    glm::vec2 movementInput{0.0f, 0.0f};

    float theta{};
    float phi{};

    float movementSpeed{5.0f};
    float currentSpeed{5.0f};

    void update() override;
};
