//
// Created by AwokenOwen on 4/13/26.
//

#include "Object.h"
#include "Component.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "WorldManager.h"

Vector3 Object::getLocalPosition() const {
    return m_localPosition;
}

void Object::setLocalPosition(const Vector3 &position) {
    // Set the new position
    m_localPosition = position;

    // Reload the model matrix
    m_localMatrix = Matrix4::ModelMatrix(m_localPosition, m_localRotation, m_localScale);
    // Set the dirty flag to reload world matrix
    setDirtyFlag();
}

Quaternion Object::getLocalRotation() const {
    return m_localRotation;
}

void Object::setLocalRotation(const Quaternion &rotation) {
    // Set the new rotation
    m_localRotation = rotation;

    // Reload the model matrix
    m_localMatrix = Matrix4::ModelMatrix(m_localPosition, m_localRotation, m_localScale);
    // Set the dirty flag to reload world matrix
    setDirtyFlag();
}

Vector3 Object::getLocalScale() const {
    return m_localScale;
}

void Object::setLocalScale(const Vector3 &scale) {
    // Set the new scale
    m_localScale = scale;

    // Reload the model matrix
    m_localMatrix = Matrix4::ModelMatrix(m_localPosition, m_localRotation, m_localScale);
    // Set the dirty flag to reload world matrix
    setDirtyFlag();
}

Matrix4 Object::getLocalMatrix() const {
    return m_localMatrix;
}

Vector3 Object::getWorldPosition() const {
    // Get the world matrix
    const Matrix4 m = getWorldMatrix();
    // Extract the translation piece
    return {m[0][3], m[1][3], m[2][3]};
}

void Object::setWorldPosition(const Vector3 &position) {
    // Check if there is a parent
    if (p_parent == nullptr)
    {
        // If not parent local position is world position so set the new position
        setLocalPosition(position);
        // All done so return
        return;
    }
    // Create the new matrix representing the translation needed
    auto m = p_parent->getWorldMatrix().inverse() * Matrix4::Translate({}, position);
    // Extract and set the new position
    setLocalPosition(Vector3(m[0][3], m[1][3], m[2][3]));
}

Quaternion Object::getWorldRotation() const {
    // Quaternion from matrix takes into account the translation and scale only exporting the rotation of the matrix
    return Quaternion(getWorldMatrix());
}

void Object::setWorldRotation(const Quaternion &rotation) {
    // If there is no parent, the local position is the world position. If not find the rotation by multiplying the rotation as a matrix to the inverse world matrix
    const Quaternion localRot = p_parent ? Quaternion(p_parent->getWorldMatrix().inverse() * rotation.toMatrix()) : rotation;
    // Set the local rotation, which also sets the dirty flag
    setLocalRotation(localRot);
}

Vector3 Object::getWorldScale() const {
    // Get the most up to date world matrix
    auto m = getWorldMatrix();
    // Get the magnitudes of the column vectors
    auto x = Vector3(m[0]).magnitude();
    auto y = Vector3(m[1]).magnitude();
    auto z = Vector3(m[2]).magnitude();
    // Return the vector 3 of those magnitudes
    return Vector3(x,y,z);
}

void Object::setWorldScale(const Vector3 &scale) {
    // Check if there's a parent
    if (p_parent == nullptr)
    {
        // If no parent the local scale is the world scale so proceed to set
        setLocalScale(scale);
        // All done so return
        return;
    }
    // Get the new scale matrix based on the inverse of the model matrix
    auto m = p_parent->getWorldMatrix().inverse() * Matrix4::Scale({}, scale);
    // Get the magnitudes of the column vectors
    auto x = Vector3(m[0]).magnitude();
    auto y = Vector3(m[1]).magnitude();
    auto z = Vector3(m[2]).magnitude();
    // Set the new scale as a vector of the x y and z components
    setLocalScale(Vector3(x,y,z));
}

Matrix4 Object::getWorldMatrix() const {
    // Check if the dirty flag is set
    if (m_dirtyFlag) {
        // If there's a parent get the world matrix of the parent to find and update the world matrix of the child
        m_worldMatrix = p_parent ? m_localMatrix * p_parent->getWorldMatrix() : m_localMatrix;
        // Set the sirty flag to false
        m_dirtyFlag = false;
    }
    // Resturn the cached world matric
    return m_worldMatrix;
}

Vector3 Object::getWorldForward() const {
    // Get the most up to date world matrix
    const Matrix4 m = getWorldMatrix();
    // Extract the forward of the matrix, negative because OpenGL is weird and -z is forward
    return Vector3(-m[0][2], -m[1][2], -m[2][2]).normalize();
}

Vector3 Object::getWorldRight() const {
    // Get the most up to date world matrix
    const Matrix4 m = getWorldMatrix();
    // Extract the right of the matrix
    return Vector3(m[0][0], m[1][0], m[2][0]).normalize();
}

Vector3 Object::getWorldUp() const {
    // Get the most up to date world matrix
    const Matrix4 m = getWorldMatrix();
    // Extract the up of the matrix
    return Vector3(m[0][1], m[1][1], m[2][1]).normalize();
}

Object * Object::getParent() const {
    return p_parent;
}

bool Object::getActiveState() const {
    return m_activeState;
}

void Object::setActiveState(const bool active) {
    // Check if the new active state is the same as the old
    if (m_activeState == active)
        // Nothing to do so return
        return;
    // Set the new active state
    m_activeState = active;
    // Check the value of active state
    if (m_activeState)
    {
        // Add to the update loop
        getScene()->addToUpdate(this, &Object::update);
        // Call the enable event
        EnableEvent.call();
    }
    else {
        // Remove from the update loop
        getScene()->removeFromUpdate(this, &Object::update);
        // Call the disable event
        DisableEvent.call();
    }
}

void Object::Translate(const Vector3& delta)
{
    setLocalPosition(m_localPosition + delta);
}

void Object::Rotate(const Quaternion& delta)
{
    // Multiplication of quaternions is like adding, because quaternions are weird
    setLocalRotation((m_localRotation * delta).normalize());
}

void Object::Scale(const Vector3& delta)
{
    setLocalScale(m_localScale + delta);
}

Scene* Object::getScene() const
{
    return m_scene;
}

void Object::destroy()
{
    // Remove from update loop
    getScene()->removeFromUpdate(this, &Object::update);

    // Loop through children
    for (const auto child : m_children)
    {
        // Tell child to destroy themselves
        child->destroy();
    }

    // Loop through components
    for (const auto component : m_components)
    {
        // Tell components to destroy themselves
        component->destroy();
        // Object handles component deletion
        delete component;
    }
    // Delete itself
    delete this;
}

void Object::update() {
    // Call the start event
    StartEvent.call();
    // Clear the event so it only happens on the first frame a listener is added
    StartEvent.clear();

    // Call the update event
    UpdateEvent.call();
}

void Object::setDirtyFlag() const
{
    // Check the dirty flag
    if (m_dirtyFlag)
        // if already set return nothing to do
        return;
    // Set the flag to true
    m_dirtyFlag = true;
    // Loop through all children
    for (const auto child : m_children)
    {
        // Tell the children to set the flag
        child->setDirtyFlag();
    }
}

nlohmann::json Object::toJson()
{
    // Create a JSON object
    nlohmann::json j;

    // Set the basic values in the JSON object
    j["Name"] = m_name;
    j["ActiveState"] = m_activeState;
    j["Position"] = m_localPosition.toJson();
    j["Rotation"] = m_localRotation.toJson();
    j["Scale"] = m_localScale.toJson();

    // Create a list of JSON objects for the components
    std::vector<nlohmann::json> components{};
    components.reserve(m_components.size());
    // Loop through all the components
    for (const auto& c : m_components)
    {
        // Push the JSON object for the component into the list
        components.push_back(c->toJson());
    }
    // Set the list in the JSON object
    j["Components"] = components;

    // Create a list of JSON objects for the children
    int untitledNumber = 0;
    std::vector<nlohmann::json> children{};
    // Loop through the children
    for (auto& object : m_children) {
        // If there is no name, name it
        if (object->m_name.empty()) {
            object->m_name = "Untitled_" + std::to_string(untitledNumber++);
        }
        // Push the JSON of the children into the list
        children.push_back(object->toJson());
    }
    // Set the list in the JSON object
    j["Children"] = children;
    // Return the JSON object
    return j;
}

Object* Object::fromJson(const nlohmann::json& j)
{
    // Create a new Object on the heap
    const auto a = new Object();

    // Set all the basic variables
    a->m_activeState = j["ActiveState"];
    a->m_name = j["Name"];
    a->setLocalPosition(Vector3(j["Position"]["x"].get<float>(), j["Position"]["y"].get<float>(),j["Position"]["z"].get<float>()));
    a->setLocalRotation(Quaternion(j["Rotation"]["x"].get<float>(), j["Rotation"]["y"].get<float>(), j["Rotation"]["z"].get<float>(), j["Rotation"]["w"].get<float>()));
    a->setLocalScale(Vector3(j["Scale"]["x"].get<float>(), j["Scale"]["y"].get<float>(), j["Scale"]["z"].get<float>()));

    // Loop through the components in the JSON
    for (const std::vector<nlohmann::json> components = j["Components"]; const auto& c : components) {
        // Load the component onto the object
        Resource.loadComponent(a, c);
    }

    // Loop through the children in the JSON
    for (const std::vector<nlohmann::json> children = j["Children"]; const auto& c : children) {
        // Create and get the child
        auto child = fromJson(c);
        // Set it's parent
        child->p_parent = a;
        // Push it into the list of children
        a->m_children.push_back(child);
    }

    // Return the new object
    return a;
}
