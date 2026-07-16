//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"
#include "Renderer.h"
#include "Object.h"
#include <iostream>

#include "LightComponent.h"
#include "ResourceManager.h"
#include "WindowManager.h"

WorldManager & WorldManager::getInstance() {
    // Make singleton
    static WorldManager instance;
    // return singleton
    return instance;
}

Object* WorldManager::instantiateObject(Object* parent)
{
    const auto a = new Object();

    m_tobeAdded.push_back(a);

    if (parent != nullptr)
    {
        a->m_name = "Child_" + std::to_string(parent->m_children.size());
        parent->m_children.push_back(a);
        a->p_parent = parent;

        Log.log("Instantiating Object %s", a->m_name.c_str());
        return a;
    }
    a->m_name = "Root_" + std::to_string(m_activeScene->m_rootObjects.size());
    m_activeScene->m_rootObjects.push_back(a);

    Log.log("Instantiating Object %s", a->m_name.c_str());
    return a;
}

Scene * WorldManager::getActiveScene() const {
    return m_activeScene;
}

void WorldManager::setActiveScene(const std::string &name) {
    // do resource stuff
    m_activeSceneName = name;
    m_activeScene = Resource.getScene(m_activeSceneName);

    Log.log("Setting active scene: %s", m_activeSceneName.c_str());
}

void WorldManager::setBaseScene(const std::string& name) {
    m_baseScene = name;
}

int WorldManager::initialize() {
    // log initialized
    Log.log("WorldManager initialized");

    return 0;
}

void WorldManager::terminate() {
    // log terminated
    Log.log("WorldManager terminated");
}

void WorldManager::awake() {
    // do important set up
    Log.log("Loading base scene...");

    Resource.loadMaterial("assets/defaultAssets/Materials/postprocess.json");

    Resource.loadScene(m_baseScene);
    setActiveScene(m_baseScene);

    Log.log("Finished loading base scene: %s", m_activeSceneName.c_str());
}

void WorldManager::update() {
    // Add all to be objects into the scene
    for (const auto object: m_tobeAdded) {
        // Add object to update event
        UpdateEvent.add(object, &Object::update);
    }
    // clear to be added so no repeats
    m_tobeAdded.clear();

    LoadEvent.callEvent();
    LoadEvent.clearEvent();

    UnloadEvent.callEvent();
    UnloadEvent.clearEvent();

    // Call all objects update functions
    UpdateEvent.callEvent();

    for (const auto light: m_activeScene->m_lightComponents)
    {
        light->activateShadowMap();
        ShadowMapDrawEvent.callEvent(light);
    }
    ShadowMapDrawEvent.clearEvent();
    Window.resetViewport();

    // Draw all drawers to the screen/framebuffer, transparent first then opaque
    Resource.activateFramebuffer("post");
    OpaqueDrawEvent.callEvent();
    TransparentDrawEvent.callEvent();
    OpaqueDrawEvent.clearEvent();
    TransparentDrawEvent.clearEvent();
    Resource.activateFramebuffer();

    drawPostprocess();

    // Call the destroy event
    DestroyEvent.callEvent();
    // Clear the destroy event so no repeats
    DestroyEvent.clearEvent();

    // Remove all to be objects from the scene
    for (const auto object: m_tobeDestroyed) {
        UpdateEvent.remove(object, &Object::update);

        Log.log("Destroying object %s", object->m_name.c_str());

        delete(object);
    }
    // clear to be destroyed so no repeats
    m_tobeDestroyed.clear();
}

void WorldManager::drawPostprocess()
{
    auto screen = Resource.getModel("post").m_meshes[0];
    auto mat = Resource.getMaterial("assets/defaultAssets/Materials/postprocess.json");

    mat.load();

    glBindVertexArray(screen.VAO());
    glDrawElements(GL_TRIANGLES, screen.indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
