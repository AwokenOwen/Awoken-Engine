//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"
#include "Renderer.h"
#include "Object.h"
#include <iostream>
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

void WorldManager::destroyObject(Object *object) {
    // Register to the to be added event
    m_destroyEvent.add(object, &Object::destroy);
    // Add to the to be destroyed list
    m_tobeDestroyed.push_back(object);
}

void WorldManager::registerRenderer(Renderer *renderer) {
    // Based on transparency register renderer to event
    if (renderer->getTransparency())
    {
        m_transparentDrawEvent.add(renderer, &Renderer::draw);
    }else
    {
        m_opaqueDrawEvent.add(renderer, &Renderer::draw);
    }
    m_loadEvent.add(renderer, &Renderer::load);
}

void WorldManager::updateTransparency(Renderer *renderer) {
    // Remove from old event and register for new
    if (renderer->getTransparency())
    {
        m_opaqueDrawEvent.remove<>(renderer, &Renderer::draw);
        m_transparentDrawEvent.add(renderer, &Renderer::draw);
    }else
    {
        m_transparentDrawEvent.remove<>(renderer, &Renderer::draw);
        m_opaqueDrawEvent.add(renderer, &Renderer::draw);
    }
}

void WorldManager::setActiveRenderer(Renderer *renderer, const bool active) {
    if (active) {
        registerRenderer(renderer);
    }else {
       if (renderer->getTransparency())
       {
           m_transparentDrawEvent.remove(renderer, &Renderer::draw);
       }else
       {
           m_opaqueDrawEvent.remove(renderer, &Renderer::draw);
       }
    }
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

void WorldManager::setObjectActiveState(Object *object, const bool active) {
    if (active) {
        m_enableEvent.add(object, &Object::enable);
        m_updateEvent.add(object, &Object::update);
    }else {
        m_disableEvent.add(object, &Object::disable);
        m_updateEvent.remove(object, &Object::update);
    }
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
        m_updateEvent.add(object, &Object::update);
    }
    // clear to be added so no repeats
    m_tobeAdded.clear();

    m_loadEvent.callEvent();
    m_loadEvent.clearEvent();

    m_unloadEvent.callEvent();
    m_unloadEvent.clearEvent();

    // Call all objects update functions
    m_updateEvent.callEvent();

    // Draw all drawers to the screen/framebuffer, transparent first then opaque
    Resource.activateFramebuffer("post");
    m_transparentDrawEvent.callEvent();
    m_opaqueDrawEvent.callEvent();
    Resource.activateFramebuffer();

    drawPostprocess();

    // Call the destroy event
    m_destroyEvent.callEvent();
    // Clear the destroy event so no repeats
    m_destroyEvent.clearEvent();

    // Remove all to be objects from the scene
    for (const auto object: m_tobeDestroyed) {
        m_updateEvent.remove(object, &Object::update);

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
