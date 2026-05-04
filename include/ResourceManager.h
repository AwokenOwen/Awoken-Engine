//
// Created by awokenowen on 5/3/26.
//

#pragma once
#include <map>

#include "glad/glad.h"
#include <vector>
#include <string>
#include "Math.h"
#include "Manager.h"

struct Material {
    Material(std::string name, unsigned int shader);

    template<typename T>
    void setUniform(const std::string& location, T value);

private:
    std::string m_name{};

    unsigned int m_shader{};
    std::vector<unsigned int> m_textures{};
};

struct Vertex {
    Vector3 Position;
    Vector3 Normal;
    Vector2 TexCoords;
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
     * @brief Function used to load meshes into the scene
     *
     * @param name The name of the mesh to be added
     * @param path The path the mesh is located at
     * @return A copy of the mesh at the path or NULL if name is already in the map
     */
    Mesh makeMesh(std::string name, std::string path);
    /**
     * @brief A function to get a loaded mesh from the mesh map
     *
     * @param name The name of the Mesh in the map
     * @return The mesh in the map or NULL if not in the map
     */
    Mesh getMesh(std::string name);

    /**
     *
     * @param name The name of the material to be added to the map
     * @param vertexPath The path to the vertex shader
     * @param fragmentPath The path to the fragment shader
     * @return A copy of the material create with those two shaders or NULL if name is already in the map
     */
    Material makeMaterial(std::string name, std::string vertexPath, std::string fragmentPath);
    /**
     * @brief A function to get the material needed from the map
     *
     * @param name The name of the material in the map
     * @return The material in the map or NULL if the name is not in the map
     */
    Material getMaterial(std::string name);

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

    std::map<std::string, Mesh> m_meshes;
};
