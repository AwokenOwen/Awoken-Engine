//
// Created by AwokenOwen on 5/3/26.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ResourceManager.h"

#include <iostream>
#include <utility>

#include "Object.h"
#include "LogManager.h"
#include "LinearMath.h"
#include "CameraComponent.h"
#include "ModelRendererComponent.h"
#include "WindowManager.h"

#include "assimp/postprocess.h"

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

void Material::load()
{
    glUseProgram(m_shaderProgram);
    for (const auto& func : m_uniforms | std::views::values)
    {
        func();
    }
    for (int i = 0; i < m_textures.size(); ++i)
    {
        setUniform(std::string("textures[" + std::to_string(i) + "]"), i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].m_textureID);
    }
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

std::string ResourceManager::registerScene(const std::string &path) {
    std::ifstream f(path);
    nlohmann::json j = nlohmann::json::parse(f);

    auto name = j["Name"].get<std::string>();

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
            return e;
        }
        return false;
    });
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

void ResourceManager::loadModel(const std::string& path)
{
    if (m_loadedModels.contains(path))
    {
        m_loadedModels.at(path).listeners++;
        return;
    }

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_PreTransformVertices | aiProcess_Triangulate | aiProcess_FlipUVs) ;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Log.logError("Could not load model, Assimp Error: '%s'", import.GetErrorString());
        return;
    }

    m_loadedModels.insert({path, {}});
    processNode(scene->mRootNode, scene, path);
}

Model ResourceManager::getModel(const std::string& path)
{
    if (!m_loadedModels.contains(path))
    {
        Log.logError("No model of path '%s' exists in the loaded map", path.c_str());
        return{};
    }

    return m_loadedModels[path];
}

void ResourceManager::unloadModel(const std::string& path)
{
    if (!m_loadedModels.contains(path))
    {
        Log.logError("No model of path '%s' exists in the loaded map", path.c_str());
        return;
    }
    m_loadedModels.at(path).listeners--;
    if (m_loadedModels.at(path).listeners <= 0)
    {
        m_loadedModels.erase(path);
    }
}

void ResourceManager::makeMesh(const std::string& name, const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    if (m_loadedModels.contains(name))
    {
        Log.logError("Model '%s' is already in the map", name.c_str());
    }

    Mesh map_mesh;
    unsigned int VAO{};
    unsigned int VBO{};
    unsigned int EBO{};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uvs)));

    glBindVertexArray(0);

    map_mesh.m_VAO = VAO;
    map_mesh.m_VBO = VBO;
    map_mesh.m_EBO = EBO;
    map_mesh.m_indices = indices;

    Model mapModel{};

    mapModel.m_meshes.push_back(map_mesh);

    m_loadedModels.insert({name, mapModel});
}


void ResourceManager::loadTexture(const std::string& path)
{
    if (m_loadedTextures.contains(path))
    {
        m_loadedTextures.at(path).listeners++;
        return;
    }

    //Loading Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height,
        &nrChannels, 0);
    if (data)
    {
        switch (nrChannels) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        }
    }
    else
    {
        Log.logError("Could not load texture '%s'", path.c_str());
    }
    stbi_image_free(data);

    auto mapTexture = Texture{};

    mapTexture.m_textureID = texture;

    m_loadedTextures.insert({path, mapTexture});
}

void ResourceManager::loadHDR(const std::string& path)
{
    glDisable(GL_CULL_FACE);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        Log.logError("Could not load hdr '%s'", path.c_str());
        return;
    }

    const auto frameBuffer = makeFramebuffer("HDR", 512, 512);

    unsigned int cubeMap;
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto captureProjection = CameraComponent::makePerspectiveMatrix(toRadians(90.0f), 1.0f, 0.1f, 10.0f);
    Matrix4 captureViews[] = {
        Matrix4::lookAt(Vector3{}, Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::lookAt(Vector3{}, Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::lookAt(Vector3{}, Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f, 0.0f,  1.0f)),
        Matrix4::lookAt(Vector3{}, Vector3(0.0f,  -1.0f,  0.0f), Vector3(0.0f, 0.0f,  -1.0f)),
        Matrix4::lookAt(Vector3{}, Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::lookAt(Vector3{}, Vector3(0.0f,  0.0f,  -1.0f), Vector3(0.0f, -1.0f,  0.0f))
    };

    loadMaterial("assets/defaultAssets/Materials/equirectangular.json");
    auto mat = getMaterial("assets/defaultAssets/Materials/equirectangular.json");
    loadModel("assets/defaultAssets/Models/cube.fbx");
    const auto model = getModel("assets/defaultAssets/Models/cube.fbx");

    mat.setUniform("projection", captureProjection);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.m_id);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glBindVertexArray(model.m_meshes[0].VAO());
    for (unsigned int i = 0; i < 6; ++i)
    {
        mat.setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glDrawElements(GL_TRIANGLES, model.m_meshes[0].indexCount(), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
    activateFramebuffer();
    Window.resetViewport();

    auto texture = Texture{};
    texture.m_textureID = cubeMap;
    texture.listeners = 0;
    texture.cubeMap = true;

    m_loadedTextures.insert({path, texture});
}

Texture ResourceManager::getTexture(const std::string& path) const
{
    if (!m_loadedTextures.contains(path))
    {
        Log.logError("No texture of path '%s' exists in the loaded map", path.c_str());
        return {};
    }

    return m_loadedTextures.at(path);
}

void ResourceManager::unloadTexture(const std::string& path)
{
    if (!m_loadedTextures.contains(path))
    {
        Log.logError("No texture of path '%s' exists in the loaded map", path.c_str());
        return;
    }

    m_loadedTextures.at(path).listeners--;
    if (m_loadedTextures.at(path).listeners <= 0)
    {
        m_loadedTextures.erase(path);
    }
}

void ResourceManager::loadMaterial(const std::string& path)
{
    if (m_loadedMaterials.contains(path))
    {
        m_loadedMaterials.at(path).listeners++;
        return;
    }

    std::ifstream f(path);
    nlohmann::json j = nlohmann::json::parse(f);

    auto vertexShaderPath = j["VertexShader"].get<std::string>();
    auto fragmentShaderPath = j["FragmentShader"].get<std::string>();

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Log.logError(e.what());
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        Log.logError(infoLog);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
       Log.logError(infoLog);
    }

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        Log.logError(infoLog);
    }

    auto mapMaterial = Material{};
    mapMaterial.m_shaderProgram = shaderProgram;

    // Load Other Uniforms
    for (const std::vector<nlohmann::json> uniforms = j["Uniforms"]; const auto& u : uniforms)
    {
        auto name = u["Name"].get<std::string>();
        auto type = u["Type"].get<std::string>();
        m_uniformMap.at(type)(mapMaterial.m_shaderProgram, u);
    }

    for (const std::vector<nlohmann::json> textures = j["Textures"]; const auto& t : textures)
    {
        Resource.loadTexture(t.get<std::string>());
        mapMaterial.m_textures.push_back(Resource.getTexture(t.get<std::string>()));
    }

    m_loadedMaterials.insert({path, mapMaterial});
}

Material ResourceManager::getMaterial(const std::string& path)
{
    if (!m_loadedMaterials.contains(path))
    {
        Log.logError("Cannot find material %s", path.c_str());
        return {};
    }
    return m_loadedMaterials.at(path);
}

void ResourceManager::unloadMaterial(const std::string& path)
{
    if (!m_loadedMaterials.contains(path))
    {
        Log.logError("Cannot find material %s", path.c_str());
        return;
    }
    m_loadedMaterials.at(path).listeners--;
    if (m_loadedMaterials.at(path).listeners <= 0)
    {
        m_loadedMaterials.erase(path);
    }
}

void ResourceManager::setMainCamera(CameraComponent* camera)
{
    if (m_mainCamera != nullptr)
    {
        m_mainCamera->m_main = false;
    }
    m_mainCamera = camera;
    m_mainCamera->m_main = true;
}

CameraComponent* ResourceManager::getMainCamera() const
{
    return m_mainCamera;
}

FrameBuffer ResourceManager::makeFramebuffer(const std::string& name, int width, int height)
{
    if (m_framebuffers.contains(name))
    {
        Log.logWarning("Framebuffer %s already exists returning...", name.c_str());
        return m_framebuffers.at(name);
    }

    if (width == -1)
    {
        width = Window.getViewportWidth();
    }
    if (height == -1)
    {
        height = Window.getViewportHeight();
    }

    auto frameBuffer = FrameBuffer{};

    glGenFramebuffers(1, &frameBuffer.m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.m_id);
    // create a color attachment texture
    glGenTextures(1, &frameBuffer.m_colorBuffer);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.m_colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer.m_colorBuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &frameBuffer.m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer.m_renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer.m_renderBuffer); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Framebuffer not complete: %d", fboStatus);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto texture = Texture{};
    texture.m_textureID = frameBuffer.m_colorBuffer;
    texture.listeners = 0;

    m_framebuffers.insert({name, frameBuffer});
    m_loadedTextures.insert({name, texture});

    return frameBuffer;
}

void ResourceManager::activateFramebuffer(const std::string& name)
{
    if (name.empty())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        return;
    }

    if (!m_framebuffers.contains(name))
    {
        Log.logError("Cannot find framebuffer %s", name.c_str());
        return;
    }
    const auto frameBuffer = m_framebuffers.at(name);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.m_id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

int ResourceManager::initialize() {
    std::ifstream f("gameInit.json");
    nlohmann::json j = nlohmann::json::parse(f);

    // Register all scenes
    const std::string primaryPath = j["Primary"].get<std::string>();
    World.setBaseScene(registerScene(primaryPath));

    for (const std::vector<nlohmann::json> children = j["Scenes"]; const auto& c : children) {
        registerScene(c.get<std::string>());
    }

    // Register all components to be read and added from JSON files
    registerComponent<CameraComponent>("Camera");
    registerComponent<ModelRendererComponent>("ModelRenderer");

    m_uniformMap.insert({"Int", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = j["Value"].get<int>();

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform1i(uniform, value);
    }});

    m_uniformMap.insert({"Float", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = j["Value"].get<float>();

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform1f(uniform, value);
    }});

    m_uniformMap.insert({"Vector2", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = Vector2::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform2f(uniform, value.x, value.y);
    }});

    m_uniformMap.insert({"Vector3", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = Vector3::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform3f(uniform, value.x, value.y, value.z);
    }});

    m_uniformMap.insert({"Vector4", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = Vector4::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform4f(uniform, value.x, value.y, value.z, value.w);
    }});

    m_uniformMap.insert({"Matrix4", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        auto value = Matrix4::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniformMatrix4fv(uniform, 1, GL_FALSE, value.toFloatArray());
    }});

    makePostprocessingScreen();

    Log.log("Resource Manager initialized");
    return 0;
}

void ResourceManager::terminate() {
    flushLoadedScenes();

    m_sceneMap.clear();
    m_loadedScenes.clear();

    Log.log("Resource Manager Terminated");
}

void ResourceManager::processNode(const aiNode* node, const aiScene* scene, const std::string& path)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, path);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, path);
    }
}

void ResourceManager::processMesh(const aiMesh* mesh, const std::string &path)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        Vector3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.m_position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.m_normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vector2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.m_uvs = vec;
        }
        else
            vertex.m_uvs = Vector2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh map_mesh;
    unsigned int VAO{};
    unsigned int VBO{};
    unsigned int EBO{};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uvs)));

    glBindVertexArray(0);

    map_mesh.m_VAO = VAO;
    map_mesh.m_VBO = VBO;
    map_mesh.m_EBO = EBO;
    map_mesh.m_indices = indices;

    m_loadedModels.at(path).m_meshes.push_back(map_mesh);
}

void ResourceManager::makePostprocessingScreen()
{
    const std::vector vertices
    {
        Vertex{Vector3(-1, -1, 0)},
        Vertex{Vector3(1, -1, 0)},
        Vertex{Vector3(1, 1, 0)},
        Vertex{Vector3(-1, 1, 0)},
    };
    const std::vector<unsigned int> indices
    {
        0, 1, 2,
        0, 2, 3,
    };

    makeMesh("post", vertices, indices);
    makeFramebuffer("post");
}
