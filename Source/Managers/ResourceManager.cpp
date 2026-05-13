//
// Created by AwokenOwen on 5/3/26.
//

#include "ResourceManager.h"

#include <iostream>
#include <utility>

#include "Object.h"
#include "LogManager.h"

#include "CameraComponent.h"
#include "MeshRendererComponent.h"

nlohmann::json Scene::toJson() const {
    nlohmann::json j;

    j["Name"] = m_name;
    std::vector<nlohmann::json> temp;
    int untitledNumber = 0;
    for (auto object : m_rootObjects) {
        if (object->m_name.empty()) {
            object->m_name = "Untitled_" + std::to_string(untitledNumber++);
        }
        temp.push_back(object->toJson());
    }

    j["Root Objects"] = temp;

    return j;
}

std::string Scene::getName() const {
    return m_name;
}

Scene * Scene::fromJson(const nlohmann::json &j) {
    auto *scene = new Scene(j["Name"].get<std::string>());
    for (const std::vector<nlohmann::json> objects = j["Root Objects"]; const auto& o : objects) {
        scene->m_rootObjects.emplace_back(Object::fromJson(o));
    }

    return scene;
}

void Scene::end() const {
    for (const auto object: m_rootObjects) {
        object->end();
    }
    delete this;
}

ResourceManager & ResourceManager::getInstance() {
    static ResourceManager single;
    return single;
}

std::string ResourceManager::addScene(const std::string &path) {
    std::ifstream f(path);
    nlohmann::json j = nlohmann::json::parse(f);
    if (j == nlohmann::json::value_t::null) {
        Log.logError("Could not load file at path %s", path.c_str());
        return {};
    }
    std::string name = j["Name"].get<std::string>();

    if (m_sceneMap.contains(name)) {
        Log.logError("Could not add scene '%s' as it's already in the scene map", name.c_str());
        return {};
    }

    m_sceneMap.insert({name, path});
    return name;
}

Scene * ResourceManager::loadScene(const std::string &name) {
    if (!m_sceneMap.contains(name)) {
        Log.logError("Could not find scene '%s' in scene map", name.c_str());
        return nullptr;
    }

    std::string path = m_sceneMap[name];

    std::ifstream f(path);
    nlohmann::json j = nlohmann::json::parse(f);

    auto scene = Scene::fromJson(j);

    m_loadedScenes.insert({name, scene});

    Log.log("Loaded scene '%s' into loaded scene map", name.c_str());

    return scene;
}

Scene * ResourceManager::getScene(const std::string &name) {
    if (!m_loadedScenes.contains(name)) {
        Log.logError("Could not find scene '%s' in loaded scenes", name.c_str());
        return nullptr;
    }
    return m_loadedScenes[name];
}

void ResourceManager::flushLoadedScenes(const std::vector<std::string>& keepLoaded) {
    auto v = keepLoaded;
    v.push_back(World.m_activeSceneName);

    std::erase_if(m_loadedScenes, [&](const auto& pair) {
        // Return true if the key should be erased (not in the vector)
        if (bool e = std::find(keepLoaded.begin(), keepLoaded.end(), pair.first) == keepLoaded.end()) {
            pair.second->end();
            return true;
        }
        return false;
    });
}

void ResourceManager::loadComponent(Object* obj, nlohmann::json component) const
{
    auto type = component["Type"].get<std::string>();

    if (!m_componentMap.contains(type))
    {
        Log.logError("Could not find registered component");
        return;
    }

    m_componentMap.at(type)(obj, component);
}

template <typename T>
void ResourceManager::registerComponent(const std::string& type)
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
    std::function lambda = [](Object* obj, nlohmann::json j)
    {
        auto a = obj->addComponent<T>();
        a->fromJson(std::move(j));
    };
    m_componentMap.insert({type, lambda});
    Log.log("Registered component '%s'", type.c_str());
}

int ResourceManager::initialize() {
    std::ifstream f("gameInit.json");
    nlohmann::json j = nlohmann::json::parse(f);

    const std::string primaryPath = j["Primary"].get<std::string>();
    World.setBaseScene(addScene(primaryPath));

    for (const std::vector<nlohmann::json> children = j["Scenes"]; const auto& c : children) {
        addScene(c.get<std::string>());
    }

    // Register all components to be read and added from JSON files
    registerComponent<CameraComponent>("Camera");
    registerComponent<MeshRendererComponent>("MeshRenderer");


    Log.log("Resource Manager initialized");
    return 0;
}

void ResourceManager::terminate() {
    flushLoadedScenes();

    m_sceneMap.clear();
    m_loadedScenes.clear();

    Log.log("Resource Manager Terminated");
}