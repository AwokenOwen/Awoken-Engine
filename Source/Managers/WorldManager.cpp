//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"
#include "Renderer.h"
#include "Object.h"
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include "ResourceManager.h"

WorldManager & WorldManager::getInstance() {
    // Make singleton
    static WorldManager instance;
    // return singleton
    return instance;
}

void WorldManager::registerObject(Object *object, const Object* parent) {
    // Add to the to be added list
    m_tobeAdded.push_back(object);
    if (parent == nullptr) {
        m_activeScene->m_rootObjects.push_back(object);
    }
}

void WorldManager::destroyObject(Object *object) {
    // Register to the to be added event
    m_activeScene->m_destroyEvent.add(object, &Object::destroy);
    // Add to the to be destroyed list
    m_tobeDestroyed.push_back(object);
}

void WorldManager::registerRenderer(Renderer *renderer) {
    // Based on transparency register renderer to event
    renderer->getTransparency() ? m_activeScene->m_transparentDrawEvent.add(renderer, &Renderer::draw) : m_activeScene->m_opaqueDrawEvent.add(renderer, &Renderer::draw);
}

void WorldManager::updateTransparency(Renderer *renderer) {
    // Remove from old event
    renderer->getTransparency() ? m_activeScene->m_opaqueDrawEvent.remove<>(renderer, &Renderer::draw) : m_activeScene->m_transparentDrawEvent.remove<>(renderer, &Renderer::draw);
    // Register to new event
    renderer->getTransparency() ? m_activeScene->m_transparentDrawEvent.add(renderer, &Renderer::draw) : m_activeScene->m_opaqueDrawEvent.add(renderer, &Renderer::draw);
}

void WorldManager::setActiveRenderer(Renderer *renderer, const bool active) {
    if (active) {
        registerRenderer(renderer);
    }else {
       renderer->getTransparency() ? m_activeScene->m_transparentDrawEvent.remove(renderer, &Renderer::draw) :  m_activeScene->m_opaqueDrawEvent.remove(renderer, &Renderer::draw);
    }
}

CameraComponent * WorldManager::getActiveCamera() {
    // Camera not a thing yet, will be added later
    return nullptr;
}

Scene * WorldManager::getActiveScene() const {
    // getter for the active scene
    return Resource.getScene(m_activeSceneName);
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

void WorldManager::setObjectActiveState(Object *object, const bool active) const {
    if (active) {
        m_activeScene->m_enableEvent.add(object, &Object::enable);
        m_activeScene->m_updateEvent.add(object, &Object::update);
    }else {
        m_activeScene->m_disableEvent.add(object, &Object::disable);
        m_activeScene->m_updateEvent.remove(object, &Object::update);
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

    Resource.loadScene(m_baseScene);
    setActiveScene(m_baseScene);

    Log.log("Finished loading base scene: %s", m_activeSceneName.c_str());
}

void WorldManager::update() {
    // Add all to be objects into the scene
    for (const auto object: m_tobeAdded) {
        // Add object to update event
        m_activeScene->m_updateEvent.add(object, &Object::update);
    }
    // clear to be added so no repeats
    m_tobeAdded.clear();

    // Call all objects update functions
    m_activeScene->m_updateEvent.callEvent();

    // Draw all drawers to the screen/framebuffer, transparent first then opaque
    m_activeScene->m_transparentDrawEvent.callEvent();
    m_activeScene->m_opaqueDrawEvent.callEvent();

    // Call the destroy event
    m_activeScene->m_destroyEvent.callEvent();
    // Clear the destroy event so no repeats
    m_activeScene->m_destroyEvent.clearEvent();

    // Remove all to be objects from the scene
    for (const auto object: m_tobeDestroyed) {
        m_activeScene->m_updateEvent.remove(object, &Object::update);
        delete(object);
    }
    // clear to be destroyed so no repeats
    m_tobeDestroyed.clear();
}
