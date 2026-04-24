//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"
#include "Renderer.h"
#include "Object.h"
#include <iostream>
#include <nlohmann/json_fwd.hpp>

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

CameraComponent * WorldManager::getActiveCamera() {
    // Camera not a thing yet, will be added later
    return nullptr;
}

Scene * WorldManager::getActiveScene() const {
    // getter for the active scene
    return m_activeScene;
}

void WorldManager::loadScene(const char *path) {
    // Log the loading scene
    Log.log("Loading %s", path);
    // Call helper for parsing JSON
    auto s = createSceneFromFile(path);
    if (s == nullptr) {
        Log.logError("Failed to load %s as into scene map", path);
        return;
    }

    // insert into the loaded map to be called on later
    m_loadedScenes.insert({s->name, s});

    // Log that scene is loaded
    Log.log("Loaded %s", s->name.c_str());
}

void WorldManager::setActiveScene(const char* path, const bool isFile) {
    // if the scene is being loaded from a file
    if (isFile) {
        // Helper to parse JSON file
        const auto s = createSceneFromFile(path);
        if (s == nullptr) {
            Log.logError("Failed to set %s as Active Scene", path);
            return;
        }
        // insert into the loaded map
        m_loadedScenes.insert({s->name, s});
        // set as active scene
        m_activeScene = s;
        // log that it's loaded
        Log.log("Loaded %s", s->name.c_str());
    }else {
        // Look for scene name in map
        if (const auto s = m_loadedScenes.find(path); s != m_loadedScenes.end()) {
            // set as active scene
            m_activeScene = s->second;
            // Log that the scene is loaded
            Log.log("Loaded %s", m_activeScene->name.c_str());
            return;
        }
        // log error if the scene querier is not in the map
        Log.logError("Scene %s not found", path);
    }
}

void WorldManager::setBaseScene(const char *name) {
    m_baseScene = name;
}

Scene* WorldManager::createSceneFromFile(const char *path) {
    std::ifstream ifs(path);
    if (!ifs) {
        Log.logError("%s, could not be opened", path);
        return nullptr;
    }
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
    // load default scene into memory
    loadScene("assets/Scenes/default.scene");

    // log initialized
    Log.log("WorldManager initialized");

    return 0;
}

void WorldManager::terminate() {
    for (const auto [key, value]: m_loadedScenes) {
        m_loadedScenes.erase(key);
        value->end();
    }

    // log terminated
    Log.log("WorldManager terminated");
}

void WorldManager::awake() {
    // do important set up
    setActiveScene(m_baseScene.c_str());
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

void Scene::end() const {
    for (const auto object: m_rootObjects) {
        object->end();
    }
    delete this;
}
