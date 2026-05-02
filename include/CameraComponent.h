//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "Component.h"
#include "Math.h"

#define PERSPECTIVE true
#define ORTHOGRAPHIC false

class CameraComponent : public Component{
public:
    explicit CameraComponent(Object* parent);

    Matrix4 getViewMatrix() const;
    Matrix4 getProjectionMatrix() const;

    void setProjectionType(bool projectionType);

private:
    bool m_projectionType{true};

    float m_fov{toRadians(90.0f)};
    float m_near{0.001f};
    float m_far{100.0f};

    float m_left{-1.0f};
    float m_right{1.0f};
    float m_top{1.0f};
    float m_bottom{-1.0f};
};
