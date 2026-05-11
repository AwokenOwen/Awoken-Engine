//
// Created by AwokenOwen on 5/3/26.
//

#pragma once
#include "glad/glad.h"
#include <vector>
#include <string>
#include <map>
#include <nlohmann/json.hpp>

#include "Math.h"
#include "Event.h"
#include "WorldManager.h"

struct ShaderPaths {
    std::string vertexPath{};
    std::string fragmentPath{};
};

struct Shader {
    std::string name{};
    unsigned int shaderProgram{};
};

struct Material {
    Material(std::string name, unsigned int shader);

private:
    std::string m_name{};

    Shader m_shader{};
    std::vector<unsigned int> m_textures{};
};

struct Vertex {
    Vector3 m_position{};
    Vector3 m_normal{};
    Vector2 m_texCoords{};

    Vertex() = default;
    explicit Vertex(Vector3 position, Vector3 normal, Vector2 texCoords);
    explicit Vertex(Vector3 position, Vector2 texCoords);
    explicit Vertex(Vector3 position);
};

struct Mesh {
    unsigned int m_VAO{};
    unsigned int m_VBO{};
    unsigned int m_EBO{};

    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void setMaterial(Material* material);

private:
    std::string m_name{};

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Material* p_material{};
};

class Object;
struct Scene {
    friend class WorldManager;
    friend class ResourceManager;

    [[nodiscard]] std::string getName() const;

private:
    std::string m_name{};

    Event<> m_updateEvent{};
    Event<> m_enableEvent{};
    Event<> m_disableEvent{};
    Event<> m_destroyEvent{};

    Event<> m_transparentDrawEvent{};
    Event<> m_opaqueDrawEvent{};

    std::vector<Object*> m_rootObjects{};

    static Scene* fromJson(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json toJson() const;

    void end() const;
};

/**
 * @brief Singleton Macro
 */
#define Resource ResourceManager::getInstance()
class ResourceManager : public Manager {
    friend class GameManager;
public:
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the ResourceManager
     */
    static ResourceManager& getInstance();

    /**
     * @brief Function used to load a scene from the JSON file path and into the scene map
     *
     * @param path the path to the JSON file containing all the information
     * @return the name of the scene if added or empty string if name of scene is already in map
     */
    std::string addScene(const std::string& path);
    /**
     * @brief load scene from sceneMap into the loaded Scene map
     *
     * @param name name of scene in scene map
     * @return Scene pointer to the newly loaded scene or nullptr if name not in scene map
     */
    Scene* loadScene(const std::string& name);
    /**
     * @brief Function that gets a scene in the loaded scene map and returns it. If the scene is not in the loaded scene map it tries to load it from scene map
     *
     * @param name name of the scene in the loaded scene map
     * @return The scene from the loaded scene map nullptr if it's not in either the scene map or loaded scene map
     */
    Scene* getScene(const std::string& name);
    /**
     * @brief clears the loaded Scene map to free ram usage
     *
     * @param keepLoaded List of scenes that will not be removed from map
     */
    void flushLoadedScenes(const std::vector<std::string>& keepLoaded = {});

private:
    /**
     * @brief Starts the Resource Manager
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    int initialize() override;
    /**
     * @brief Shuts down the Resource Manager freeing and necessary data
     */
    void terminate() override;
    /**
     * @brief Private default constructor for singleton functionality
     */
    ResourceManager() = default;
    /**
     * @brief Private default deconstructor for singleton functionality
     */
    ~ResourceManager() override = default;

    std::map<std::string, std::string> m_sceneMap;
    std::map<std::string, Scene*> m_loadedScenes;
};