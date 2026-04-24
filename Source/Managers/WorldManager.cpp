//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"

#include <iostream>
#include <nlohmann/json_fwd.hpp>

#include "Renderer.h"
#include "Object.h"

WorldManager & WorldManager::getInstance() {
    static WorldManager instance;
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

CameraComponent * WorldManager::getActiveCamera() {
    return nullptr;
}

Scene * WorldManager::getActiveScene() {
    return m_activeScene;
}

void WorldManager::loadScene(const char *path) {
    Log.log("Loading %s", path);
    auto s = createSceneFromFile(path);

    m_loadedScenes.insert({s->name, s});

    Log.log("Loaded %s", s->name.c_str());
}

void WorldManager::setActiveScene(const char* path, const bool isFile) {
    if (isFile) {
        const auto s = createSceneFromFile(path);
        m_loadedScenes.insert({s->name, s});
        m_activeScene = s;
        Log.log("Loaded %s", s->name.c_str());
    }else {
        if (const auto s = m_loadedScenes.find(path); s != m_loadedScenes.end()) {
            m_activeScene = s->second;
            Log.log("Loaded %s", m_activeScene->name.c_str());
            return;
        }
        Log.logError("Scene %s not found", path);
    }
}

Scene* WorldManager::createSceneFromFile(const char *path) {
    std::ifstream ifs(path);
    nlohmann::json j = nlohmann::json::parse(ifs);

    auto a = new Scene();

    a->name = j["Name"];

    for (std::vector<nlohmann::json> rootObjects = j["Root Objects"]; const auto object: rootObjects) {
        auto root = Object::fromJson(object);
        a->m_rootObjects.push_back(root);
        a->m_startEvent.add(root, &Object::start);
        a->m_updateEvent.add(root, &Object::update);
        for (const auto child: root->m_children) {
            a->m_startEvent.add(child, &Object::start);
            a->m_updateEvent.add(child, &Object::update);
        }
    }

    ifs.close();

    return a;
}

int WorldManager::initialize() {
    Log.log("WorldManager initialized");

    return 0;
}

void WorldManager::terminate() {
    for (const auto scene : m_loadedScenes) {
        delete &scene;
    }

    Log.log("WorldManager terminated");
}

void WorldManager::awake() {
    // do important set up
}

void WorldManager::update() {
    // Add all to be objects into the scene
    for (const auto object: m_tobeAdded) {
        // Add object to start event
        m_activeScene->m_startEvent.add(object, &Object::start);
        // Add object to update event
        m_activeScene->m_updateEvent.add(object, &Object::update);
    }
    // clear to be added so no repeats
    m_tobeAdded.clear();

    // Call all the new objects start function
    m_activeScene->m_startEvent.callEvent(this);
    // Clear event so no repeats
    m_activeScene->m_startEvent.clearEvent(this);

    // Call all objects update functions
    m_activeScene->m_updateEvent.callEvent(this);

    // Draw all drawers to the screen/framebuffer, transparent first then opaque
    m_activeScene->m_transparentDrawEvent.callEvent(this);
    m_activeScene->m_opaqueDrawEvent.callEvent(this);

    // Call the destroy event
    m_activeScene->m_destroyEvent.callEvent(this);
    // Clear the destroy event so no repeats
    m_activeScene->m_destroyEvent.clearEvent(this);

    // Remove all to be objects from the scene
    for (const auto object: m_tobeDestroyed) {
        m_activeScene->m_updateEvent.remove(object, &Object::update);
        delete(object);
    }
    // clear to be destroyed so no repeats
    m_tobeDestroyed.clear();
}

nlohmann::json Scene::toJson() const {
    nlohmann::json j;

    j["Name"] = name;

    int untitledNumber = 0;
    std::vector<nlohmann::json> rootObjects;
    for (auto& object : m_rootObjects) {
        if (object->name == "") {
            object->name = "Untitled_" + std::to_string(untitledNumber++);
        }
        rootObjects.push_back(object->toJson());
    }

    j["Root Objects"] = rootObjects;

    return j;
}
