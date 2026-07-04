//
// Created by AwokenOwen on 5/3/26.
//

#pragma once
#include "glad/glad.h"
#include <vector>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <functional>

#include "LinearMath.h"
#include "WorldManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "AL/al.h"

constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class LightComponent;

constexpr std::size_t NUM_BUFFERS = 4;
constexpr std::size_t BUFFER_SIZE = 65536; // 32kb of data in each buffer
struct Sound
{
    friend class ResourceManager;
    ALuint m_ID[NUM_BUFFERS];
    ALenum m_format{};
    std::int32_t m_sampleRate{};
    std::vector<char> m_soundData{};
private:
    int listeners{1};
};

enum TextureType
{
    PNG = 0,
    JPEG = 1,
    HDR = 2,
    CUBEMAP = 3,
};

struct FrameBuffer
{
    unsigned int m_id;
    unsigned int m_colorBuffer;
    unsigned int m_renderBuffer;
};

struct Texture
{
    friend class ResourceManager;
    unsigned int m_textureID{};
    bool cubeMap{false};
private:
    int listeners{1};
};

enum MaterialType
{
    NONE = 0,
    UNLIT = 1,
    DEFAULT_LIT = 2,
    DEFAULT_SPRITE = 3,
};

struct Material
{
    friend class ResourceManager;
    void load();
    [[nodiscard]] MaterialType getType() const
    {
        return m_type;
    };
    template <typename T>
    void setUniform(const std::string& name, T value)
    {
        glUseProgram(m_shaderProgram);

        auto uniform = glGetUniformLocation(m_shaderProgram, name.c_str());

        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>)
            glUniform1i(uniform, static_cast<int>(value));
        else if constexpr (std::is_same_v<T, float>)
            glUniform1f(uniform, value);
        else if constexpr (std::is_same_v<T, Vector2>)
            glUniform2f(uniform, value.x, value.y);
        else if constexpr (std::is_same_v<T, Vector3>)
            glUniform3f(uniform, value.x, value.y, value.z);
        else if constexpr (std::is_same_v<T, Vector4>)
            glUniform4f(uniform, value.x, value.y, value.z, value.w);
        else if constexpr (std::is_same_v<T, Matrix4>)
            glUniformMatrix4fv(uniform, 1, GL_FALSE, value.toFloatArray());
    }
private:
    unsigned int m_shaderProgram{};
    std::map<std::string, std::function<void()>> m_uniforms{};
    std::map<std::string, Texture> m_textures{};
    MaterialType m_type;
    int listeners{1};
};

struct Vertex
{
    Vector3 m_position{};
    Vector3 m_normal{};
    Vector2 m_uvs{};
};

struct BoundingBox
{
    Vector3 min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    Vector3 max{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

    BoundingBox operator*(const Matrix4& modelMatrix);
};
struct Mesh
{
    friend class ResourceManager;
    [[nodiscard]] int indexCount() const
    {
        return static_cast<int>(m_indices.size());
    }
    [[nodiscard]] unsigned int VAO() const
    {
        return m_VAO;
    };
    [[nodiscard]] unsigned int VBO() const
    {
        return m_VBO;
    };
    [[nodiscard]] unsigned int EBO() const
    {
        return m_EBO;
    };
private:
    unsigned int m_VAO{};
    unsigned int m_VBO{};
    unsigned int m_EBO{};
    std::vector<unsigned int> m_indices{};
};
struct Model
{
    friend class ResourceManager;
    friend class ModelRendererComponent;
    std::vector<Mesh> m_meshes{};
    [[nodiscard]] int meshCount() const
    {
        return static_cast<int>(m_meshes.size());
    }
private:
    BoundingBox m_boundingBox{};
    int listeners{1};
};

class Object;
class CameraComponent;
struct Scene {
    friend class WorldManager;
    friend class ResourceManager;

    explicit Scene(const std::string& name)
    {
        m_name = name;
    };

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] nlohmann::json toJson() const;

    std::vector<Object*> m_rootObjects{};

    void setReflectiveMap(const std::string& name);
private:
    std::string m_name{};
    std::string m_reflectionMapName{"assets/defaultAssets/Skybox/skybox.hdr"};
    std::vector<LightComponent*> m_lightComponents{};

    static Scene* fromJson(const nlohmann::json& j);

    void end() const;
};

/**
 * @brief Singleton Macro
 */
#define Resource ResourceManager::getInstance()
class ResourceManager : public Manager {
    friend class GameManager;
    friend class WorldManager;
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
    std::string registerScene(const std::string& path);
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

    /**
     * @brief ent JSON and loads that component onto the object from the JSON file
     *
     * @param obj The object that the component will be added to
     * @param component The JSON file that will be converted into a component
     */
    void loadComponent(Object* obj, nlohmann::json component) const;
    /**
     * @brief Function at the start of the engine to register components that will be added via the JSON file
     *
     * @tparam T The class of the new component
     * @param type The name in the JSON of the component
     */
    template<typename T>
    void registerComponent(const std::string& type);

    /**
     * @brief Function that loads a model into memory
     *
     * @param path The path to the model
     */
    void loadModel(const std::string& path);
    /**
     * @brief Function to get the VAO, VBO, EBO of the model
     *
     * @param path The path to the model in memory
     * @return The model struct in memory
     */
    [[nodiscard]] Model getModel(const std::string& path);
    /**
     * @brief Function to unload a model from memory, when there are no listeners the texture gets model from memory
     *
     * @param path The path to the model in memory
     */
    void unloadModel(const std::string& path);

    void makeMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Function to load a texture into memory from a path
     *
     * @param path The path to the texture
     */
    void loadTexture(const std::string& path);
    void loadHDR(const std::string& path);
    /**
     * @brief Function to get a loaded texture from memory
     *
     * @param path path to the texture and key in texture map
     * @return The texture struct containing the unsigned int to the texture in OpenGL memory
     */
    [[nodiscard]] Texture getTexture(const std::string& path) const;
    /**
     * @brief Function to unload a texture from memory, when there are no listeners the texture gets removed from memory
     *
     * @param path Path to the texture and key in the texture map
     */
    void unloadTexture(const std::string& path);

    void loadMaterial(const std::string& path);
    Material getMaterial(const std::string& path);
    void unloadMaterial(const std::string& path);

    void setMainCamera(CameraComponent* camera);
    [[nodiscard]] CameraComponent* getMainCamera() const;

    FrameBuffer makeFramebuffer(const std::string& name, int width = -1, int height = -1);
    void activateFramebuffer(const std::string& name = "") const;
    void resizeFrameBuffer(const std::string& name, int width, int height);

    void makeIrradiancePrefilterMap(unsigned int cubeMap);
    void updateIrradiancePrefilterMap(unsigned int cubeMap);

    void loadLights(Material mat);

    void loadSound(std::string& path);
    [[nodiscard]] Sound getSound(const std::string& path) const;

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

    /**
     * @brief Helper function for loading models
     *
     * @param node Assimp node
     * @param scene Assimp scene
     * @param path path of the model in the model map
     */
    void processNode(const aiNode* node, const aiScene* scene, const std::string &path);
    /**
     * @brief Helper function for loading models
     *
     * @param mesh Assimp mesh
     * @param path path of the model in the model map
     */
    void processMesh(const aiMesh* mesh, const std::string &path);

    void loadUniformMap();

    void makePostprocessingScreen();

    void makeBRDFMap();

    void makeShadowMap(LightComponent* light);

    std::int32_t convert_to_int(const char *buffer, std::size_t len);
    bool load_wav_file_header(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, ALsizei& size);
    bool load_wav(const std::string& filename, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, std::vector<char>&);

    std::map<std::string, std::string> m_sceneMap{};
    std::map<std::string, Scene*> m_loadedScenes{};

    std::map<std::string, std::function<void(Object* obj, nlohmann::json j)>> m_componentMap{};

    std::map<std::string, Model> m_loadedModels{};
    std::map<std::string, Texture> m_loadedTextures{};
    std::map<std::string, Material> m_loadedMaterials{};

    std::map<std::string, std::function<void(unsigned int shaderProgram, nlohmann::json j)>> m_uniformMap{};

    CameraComponent* m_mainCamera{};

    std::map<std::string, FrameBuffer> m_framebuffers{};

    std::map<std::string, Sound> m_loadedSounds{};
};
