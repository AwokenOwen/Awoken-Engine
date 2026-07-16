//
// Created by AwokenOwen on 7/16/26.
//

#include "ObjectUI.h"

#include "ComponentUI.h"
#include "ResourceManager.h"
#include "WorldManager.h"

Vector2 ObjectUI::getLocalPosition()
{
    return m_localPosition;
}

void ObjectUI::setLocalPosition(const Vector2& localPosition)
{
    m_localPosition = localPosition;
}

float ObjectUI::getLocalRotation() const
{
    return m_localRotation;
}

void ObjectUI::setLocalRotation(const float localRotation)
{
    m_localRotation = localRotation;
}

Vector2 ObjectUI::getLocalScale()
{
    return m_localScale;
}

void ObjectUI::setLocalScale(const Vector2& localScale)
{
    m_localScale = localScale;
}

Matrix4 ObjectUI::getLocalMatrix()
{
    return Matrix4::ModelMatrix(Vector3(m_localPosition), Quaternion(Vector3(0,m_localRotation,0)), Vector3(0,0,1));
}

Vector2 ObjectUI::getWorldPosition()
{
    const Matrix4 m = getWorldMatrix();
    return {m[0][3], m[1][3]};
}

void ObjectUI::setWorldPosition(const Vector2& worldPosition)
{
    m_localPosition = Vector2(getWorldMatrix().inverse() * Vector4(worldPosition));
}

float ObjectUI::getWorldRotation()
{
    const Matrix4 worldRotation = getWorldMatrix() * Quaternion(Vector3(0,m_localRotation,0)).toMatrix();

    return Quaternion(worldRotation).eulerAngles().y();
}

void ObjectUI::setWorldRotation(const float worldRotation)
{
    auto q = Quaternion(Vector3(0,worldRotation,0)).toMatrix();

    const Matrix4 worldRotationMatrix = getWorldMatrix().inverse() * q;

    m_localRotation = Quaternion(worldRotationMatrix).eulerAngles().y();
}

Vector2 ObjectUI::getWorldScale()
{
    return Vector2(getWorldMatrix() * Vector4(m_localScale));
}

void ObjectUI::setWorldScale(const Vector2& worldScale)
{
    m_localScale = Vector3(getWorldMatrix().inverse() * Vector4(worldScale));
}

Matrix4 ObjectUI::getWorldMatrix()
{
    Matrix4 worldMatrix = getLocalMatrix();
    ObjectUI* currentParent = p_parent;
    while(currentParent != nullptr) {
        worldMatrix = worldMatrix * currentParent->getWorldMatrix();
        currentParent = currentParent->p_parent;
    }
    return worldMatrix;
}

ObjectUI* ObjectUI::getParent() const
{
    return p_parent;
}

void ObjectUI::setActiveState(bool active)
{
    if (m_activeState == active)
        return;
    active ? World.addUpdateEvent(this, &ObjectUI::update) : World.removeUpdateEvent(this, &ObjectUI::update);
    m_activeState = active;
}

void ObjectUI::setComponentActiveState(ComponentUI* component, bool active)
{
    if (active) {
        EnableEvent.add(component, &ComponentUI::enable);
        UpdateEvent.add(component, &ComponentUI::update);
    }else {
        DisableEvent.add(component, &ComponentUI::disable);
        UpdateEvent.remove(component, &ComponentUI::update);
    }
}

void ObjectUI::update()
{
    StartEvent.callEvent();
    StartEvent.clearEvent();

    UpdateEvent.callEvent();
}

void ObjectUI::enable()
{
    EnableEvent.callEvent();
}

void ObjectUI::disable()
{
    DisableEvent.callEvent();
}

void ObjectUI::destroy()
{
    DestroyEvent.callEvent();
}

void ObjectUI::end()
{
    for (const auto child : m_children) {
        child->end();
    }
    DestroyEvent.callEvent();

    World.removeUpdateEvent(this, &ObjectUI::update);
    World.removeDestroyEvent(this, &ObjectUI::destroy);
    delete this;
}

nlohmann::json ObjectUI::toJson()
{
    nlohmann::json j;

    j["Name"] = m_name;

    j["ActiveState"] = m_activeState;

    j["Position"] = m_localPosition.toJson();

    j["Rotation"] = m_localRotation;

    j["Scale"] = m_localScale.toJson();

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

ObjectUI* ObjectUI::fromJson(const nlohmann::json& j)
{
    const auto a = new ObjectUI();

    a->m_activeState = j["ActiveState"];

    a->m_name = j["Name"];

    a->m_localPosition = Vector3(j["Position"]["x"].get<float>(), j["Position"]["y"].get<float>(),j["Position"]["z"].get<float>());
    a->m_localRotation = j["Rotation"].get<float>();
    a->m_localScale = Vector3(j["Scale"]["x"].get<float>(), j["Scale"]["y"].get<float>(), j["Scale"]["z"].get<float>());

    for (const std::vector<nlohmann::json> components = j["Components"]; const auto& c : components) {
        //Resource.loadComponent(a, c);
    }

    for (const std::vector<nlohmann::json> children = j["Children"]; const auto& c : children) {
        auto child = fromJson(c);
        child->p_parent = a;
        a->m_children.push_back(child);
    }

    World.addUpdateEvent(a, &ObjectUI::update);

    return a;
}
