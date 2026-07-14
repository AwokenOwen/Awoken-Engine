//
// Created by AwokenOwen on 4/13/26.
//

#include "Object.h"
#include "Component.h"
#include "ResourceManager.h"
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
    const Matrix4 m = getWorldMatrix();
    return Vector3(m.a4, m.b4, m.c4);
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
    const Matrix4 m = getWorldMatrix();
    return Vector3(-m.a3, -m.b3, -m.c3).normalize();
}

Vector3 Object::getWorldRight() const {
    const Matrix4 m = getWorldMatrix();
    return Vector3(m.a1, m.b1, m.c1).normalize();
}

Vector3 Object::getWorldUp() const {
    const Matrix4 m = getWorldMatrix();
    return Vector3(m.a2, m.b2, m.c2).normalize();
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
    active ? World.addUpdateEvent(this, &Object::update) : World.removeUpdateEvent(this, &Object::update);
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

void Object::setObjectType(const ObjectType type)
{
    m_objectType = type;
}

ObjectType Object::getObjectType() const
{
    return m_objectType;
}

void Object::update() {
    StartEvent.callEvent();
    StartEvent.clearEvent();

    UpdateEvent.callEvent();
}

void Object::enable() {
    EnableEvent.callEvent();
}

void Object::disable() {
    DisableEvent.callEvent();
}

void Object::destroy() {
    DestroyEvent.callEvent();
}

void Object::end() {
    for (const auto child : m_children) {
        child->end();
    }
    DestroyEvent.callEvent();

    World.removeUpdateEvent(this, &Object::update);
    World.removeDestroyEvent(this, &Object::destroy);
    delete this;
}

nlohmann::json Object::toJson()
{
    nlohmann::json j;

    j["Name"] = m_name;

    j["ActiveState"] = m_activeState;

    j["Position"]["x"] = m_localPosition.x;
    j["Position"]["y"] = m_localPosition.y;
    j["Position"]["z"] = m_localPosition.z;

    j["Rotation"]["x"] = m_localRotation.x;
    j["Rotation"]["y"] = m_localRotation.y;
    j["Rotation"]["z"] = m_localRotation.z;
    j["Rotation"]["w"] = m_localRotation.w;

    j["Scale"]["x"] = m_localScale.x;
    j["Scale"]["y"] = m_localScale.y;
    j["Scale"]["z"] = m_localScale.z;

    std::vector<nlohmann::json> components{};
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

    World.addUpdateEvent(a, &Object::update);

    return a;
}
