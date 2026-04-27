//
// Created by awokenowen on 4/24/26.
//

#include "CameraComponent.h"

#include "Object.h"
#include "WindowManager.h"

CameraComponent::CameraComponent(Object *parent) : Component(parent) {
}

Matrix4 CameraComponent::getViewMatrix() const {
    if (getParent() == nullptr) {
        return Matrix4{};
    }
    return Matrix4{};
}

Matrix4 CameraComponent::getProjectionMatrix() const {
    if (m_projectionType) {
        return {
            cot(m_fov / 2.0f) / Window.getAspectRatio(), 0, 0, 0,
            0, cot(m_fov / 2.0f), 0, 0,
            0, 0, (m_near + m_far)/(m_near - m_far), -1,
            0, 0, (2 * m_near * m_far)/(m_near - m_far), 0
        };
    }
    return {

    };
}

void CameraComponent::setProjectionType(const bool projectionType) {
    m_projectionType = projectionType;
}
