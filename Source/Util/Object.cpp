//
// Created by AwokenOwen on 4/13/26.
//

#include "Object.h"
#include "WorldManager.h"

Vector3 Object::getLocalPosition() const {
    return m_localPosition;
}

void Object::setLocalPosition(const Vector3 &position) {
    m_localPosition = position;
}

Quaternion Object::getLocalRotation() const {
    return m_localRotation;
}

void Object::setLocalRotation(const Quaternion &rotation) {
    m_localRotation = rotation;
}

Vector3 Object::getLocalScale() const {
    return m_localScale;
}

void Object::setLocalScale(const Vector3 &scale) {
    m_localScale = scale;
}

Matrix4 Object::getLocalMatrix() const {
    return Matrix4::makeModelMatrix(m_localPosition, m_localRotation, m_localScale);
}

Vector3 Object::getWorldPosition() const {
    return Vector3(getWorldMatrix() * Vector4(m_localPosition));
}

void Object::setWorldPosition(const Vector3 &position) {
    m_localPosition = Vector3(getWorldMatrix().inverse() * Vector4(position));
}

Quaternion Object::getWorldRotation() const {
    const Matrix4 worldRotation = getWorldMatrix() * m_localRotation.toMatrix();

    return Quaternion(worldRotation);
}

void Object::setWorldRotation(const Quaternion &rotation) {
    const Matrix4 worldRotation = getWorldMatrix().inverse() * rotation.toMatrix();

    m_localRotation = Quaternion(worldRotation);
}

Vector3 Object::getWorldScale() const {
    return Vector3(getWorldMatrix() * Vector4(m_localScale));
}

void Object::setWorldScale(const Vector3 &scale) {
    m_localScale = Vector3(getWorldMatrix().inverse() * Vector4(scale));
}

Matrix4 Object::getWorldMatrix() const {
    Matrix4 worldMatrix = getLocalMatrix();
    const Object* currentParent = p_parent;
    while(currentParent != nullptr) {
        worldMatrix = worldMatrix * currentParent->getWorldMatrix();
        currentParent = currentParent->p_parent;
    }
    return worldMatrix;
}

Vector3 Object::getWorldForward() const {
    return Vector3(getWorldMatrix() * Vector4(Vector3::forward()));
}

Vector3 Object::getWorldRight() const {
    return Vector3(getWorldMatrix() * Vector4(Vector3::right()));
}

Vector3 Object::getWorldUp() const {
    return Vector3(getWorldMatrix() * Vector4(Vector3::up()));
}

Object * Object::getParent() const {
    return p_parent;
}

bool Object::getActiveState() const {
    return m_activeState;
}

void Object::setActiveState(const bool active) {
    if (m_activeState == active)
        return;
    m_activeState = active;
    World.setObjectActiveState(this, active);
}

void Object::addChild(Object *child) {
    const Object* currentParent = p_parent;

    while (currentParent != nullptr) {
        if (currentParent == child) {
            Log.logError("Cyclical Hierarchy detected, addChild failed");
            return;
        }
        currentParent = currentParent->p_parent;
    }

    m_children.push_back(child);
    child->p_parent = this;
}

void Object::setComponentActiveState(Component *component, const bool active) {
    if (active) {
        m_enableEvent.add(component, &Component::enable);
        m_updateEvent.add(component, &Component::update);
    }else {
        m_disableEvent.add(component, &Component::disable);
        m_updateEvent.remove(component, &Component::update);
    }
}

void Object::update() {
    m_startEvent.callEvent();
    m_startEvent.clearEvent();

    m_updateEvent.callEvent();
}

void Object::enable() {
    m_enableEvent.callEvent();
}

void Object::disable() {
    m_disableEvent.callEvent();
}

void Object::destroy() {
    m_destroyEvent.callEvent();
}

void Object::end() {
    for (const auto child : m_children) {
        child->end();
    }
    m_destroyEvent.callEvent();
    delete this;
}
