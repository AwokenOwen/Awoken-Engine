//
// Created by AwokenOwen on 4/13/26.
//

#include "Object.h"
#include "Component.h"
#include "ResourceManager.h"
#include "WindowManager.h"
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
    auto position = m_localPosition;
    auto rotation = m_localRotation;
    auto scale = m_localScale;

    return Matrix4::ModelMatrix(position, rotation, scale);
}

Vector3 Object::getWorldPosition() const {
    const Matrix4 m = getWorldMatrix();
    return {m[0][3], m[1][3], m[2][3]};
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
    Object* currentParent = p_parent;
    while(currentParent != nullptr) {
        worldMatrix = worldMatrix * currentParent->getWorldMatrix();
        currentParent = currentParent->p_parent;
    }
    return worldMatrix;
}

Vector3 Object::getWorldForward() const {
    const Matrix4 m = getWorldMatrix();
    return Vector3(-m[0][2], -m[1][2], -m[2][2]).normalize();
}

Vector3 Object::getWorldRight() const {
    const Matrix4 m = getWorldMatrix();
    return Vector3(m[0][0], m[1][0], m[2][0]).normalize();
}

Vector3 Object::getWorldUp() const {
    const Matrix4 m = getWorldMatrix();
    return Vector3(m[0][1], m[1][1], m[2][1]).normalize();
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
    //active ? World.addUpdateEvent(this, &Object::update) : World.removeUpdateEvent(this, &Object::update);
    m_activeState = active;
}

void Object::setComponentActiveState(Component *component, const bool active) {
    if (active) {
        EnableEvent.add(component, &Component::enable);
        UpdateEvent.add(component, &Component::update);
    }else {
        DisableEvent.add(component, &Component::disable);
        UpdateEvent.remove(component, &Component::update);
    }
}

void Object::Translate(const Vector3& translation)
{
    m_localPosition += translation;
}

Scene* Object::getScene() const
{
    return m_scene;
}

void Object::update() {
    StartEvent.call();
    StartEvent.clear();

    UpdateEvent.call();
}

void Object::enable() {
    EnableEvent.call();
}

void Object::disable() {
    DisableEvent.call();
}

void Object::destroy() {
    DestroyEvent.call();
}

void Object::end() {
    for (const auto child : m_children) {
        child->end();
    }
    DestroyEvent.call();

    //World.removeUpdateEvent(this, &Object::update);
    //World.removeDestroyEvent(this, &Object::destroy);
    delete this;
}

nlohmann::json Object::toJson()
{
    nlohmann::json j;

    j["Name"] = m_name;

    j["ActiveState"] = m_activeState;

    j["Position"] = m_localPosition.toJson();

    j["Rotation"] = m_localRotation.toJson();

    j["Scale"] = m_localScale.toJson();

    std::vector<nlohmann::json> components{};
    components.reserve(m_components.size());
    for (const auto& c : m_components)
    {
        components.push_back(c->toJson());
    }

    j["Components"] = components;

    int untitledNumber = 0;
    std::vector<nlohmann::json> children{};
    for (auto& object : m_children) {
        if (object->m_name.empty()) {
            object->m_name = "Untitled_" + std::to_string(untitledNumber++);
        }
        children.push_back(object->toJson());
    }

    j["Children"] = children;
    return j;
}

Object* Object::fromJson(const nlohmann::json& j)
{
    const auto a = new Object();

    a->m_activeState = j["ActiveState"];

    a->m_name = j["Name"];

    a->m_localPosition = Vector3(j["Position"]["x"].get<float>(), j["Position"]["y"].get<float>(),j["Position"]["z"].get<float>());
    a->m_localRotation = Quaternion(j["Rotation"]["x"].get<float>(), j["Rotation"]["y"].get<float>(), j["Rotation"]["z"].get<float>(), j["Rotation"]["w"].get<float>());
    a->m_localScale = Vector3(j["Scale"]["x"].get<float>(), j["Scale"]["y"].get<float>(), j["Scale"]["z"].get<float>());

    for (const std::vector<nlohmann::json> components = j["Components"]; const auto& c : components) {
        Resource.loadComponent(a, c);
    }

    for (const std::vector<nlohmann::json> children = j["Children"]; const auto& c : children) {
        auto child = fromJson(c);
        child->p_parent = a;
        a->m_children.push_back(child);
    }

    return a;
}
