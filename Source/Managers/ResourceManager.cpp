//
// Created by AwokenOwen on 5/3/26.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ResourceManager.h"

#include <iostream>
#include <utility>

#include "AudioSourceComponent.h"
#include "Object.h"
#include "LogManager.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "ModelRendererComponent.h"
#include "WindowManager.h"

#include "assimp/postprocess.h"

#include <ft2build.h>

#include "ParticleSystemComponent.h"

#include FT_FREETYPE_H

#include "TextRendererComponent.h"

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

    j["ReflectionMap"] = m_reflectionMapName;

    return j;
}

void Scene::setReflectiveMap(const std::string& name)
{
    m_reflectionMapName = name;
    Resource.loadHDR(name);
    Resource.makeIrradiancePrefilterMap(Resource.getTexture(name).m_textureID);
}

void Scene::addCamera(CameraComponent* camera)
{
    m_cameraComponents.push_back(camera);
}

void Scene::addToDraw(const std::function<void()>& function, const bool transparent) const
{
    for (const auto camera : m_cameraComponents)
    {
        camera->addToDraw(function, transparent);
    }
}

void Material::load()
{
    glUseProgram(m_shaderProgram);

    int i = 0;
    for (const auto& [key, texture] : m_textures)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        setUniform<int>(std::string(key), i);
        glBindTexture(texture.m_type, texture.m_textureID);

        i++;
    }
}

BoundingBox BoundingBox::operator*(const Matrix4& modelMatrix)
{
    auto _min = Vector4(min);
    auto _max = Vector4(max);

    min = Vector3(modelMatrix * _min);
    max = Vector3(modelMatrix * _max);

    return *this;
}

std::string Scene::getName() const {
    return m_name;
}

Scene * Scene::fromJson(const nlohmann::json &j) {
    auto *scene = new Scene(j["Name"].get<std::string>());

    scene->m_reflectionMapName = j["ReflectionMap"].get<std::string>();
    Resource.loadHDR(scene->m_reflectionMapName);
    Resource.makeIrradiancePrefilterMap(Resource.getTexture(scene->m_reflectionMapName).m_textureID);

    for (const std::vector<nlohmann::json> objects = j["Root Objects"]; const auto& o : objects) {
        auto obj = Object::fromJson(o);
        obj->m_scene = scene;
        scene->m_rootObjects.emplace_back(obj);
        scene->addToUpdate(obj, &Object::update);
    }
    return scene;
}

void Scene::destroy() const
{
    for (const auto object : m_rootObjects)
    {
        object->destroy();
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

    const std::string path = m_sceneMap[name];

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

void ResourceManager::flushLoadedScenes(const std::vector<std::string>& keepLoaded)
{
    std::erase_if(m_loadedScenes, [&](const auto& pair) {
        // Return true if the key should be erased (not in the vector)
        if (bool e = std::find(keepLoaded.begin(), keepLoaded.end(), pair.first) == keepLoaded.end()) {
            pair.second->destroy();
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

    auto captureProjection = Matrix4::PerspectiveMatrix(toRadians(90.0f), 1.0f, 0.1f, 10.0f);
    Matrix4 captureViews[] = {
        Matrix4::LookAt(Vector3{}, Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f, 0.0f,  1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  -1.0f,  0.0f), Vector3(0.0f, 0.0f,  -1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  -1.0f), Vector3(0.0f, -1.0f,  0.0f))
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
    mat.setUniform("hdr", 0);

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
    texture.m_type = CUBEMAP;

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

    auto geometryShaderPath = j["GeometryShader"].get<std::string>();
    unsigned int geometryShader;

    if (!geometryShaderPath.empty())
    {
        std::string geometryShaderCode{};
        std::ifstream gShaderFile;

        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            gShaderFile.open(geometryShaderPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryShaderCode = gShaderStream.str();
        }catch (std::ifstream::failure e) {
            Log.logError(e.what());
        }

        const char* geometryShaderSource = geometryShaderCode.c_str();


        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
            Log.logError(infoLog);
        }
    }

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (!geometryShaderPath.empty()) {
        glAttachShader(shaderProgram, geometryShader);
    }
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
        auto location = t["Path"].get<std::string>();
        Resource.loadTexture(location);
        mapMaterial.m_textures.insert({t["Name"].get<std::string>(), Resource.getTexture(location)});
    }

    mapMaterial.m_type = static_cast<MaterialType>(j["Type"].get<int>());

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

void ResourceManager::activateFramebuffer(const std::string& name) const {
    if (name.empty())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
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
    glEnable(GL_DEPTH_TEST);
}

void ResourceManager::resizeFrameBuffer(const std::string& name, const int width, const int height)
{
    if (!m_framebuffers.contains(name))
    {
        Log.logError("Cannot find framebuffer %s", name.c_str());
    };

    auto& frameBuffer = m_framebuffers.at(name);

    // Delete old GPU resources
    glDeleteRenderbuffers(1, &frameBuffer.m_renderBuffer);
    glDeleteTextures(1, &frameBuffer.m_colorBuffer);
    glDeleteFramebuffers(1, &frameBuffer.m_id);

    // Remove from maps so makeFramebuffer doesn't early-return
    m_loadedTextures.erase(name);
    m_framebuffers.erase(name);

    // Recreate at new size
    makeFramebuffer(name, width, height);
}

void ResourceManager::resizeCameraBuffers()
{
    for (const auto scene : m_loadedScenes | std::views::values)
    {
        for (const auto camera : scene->m_cameraComponents)
        {
            camera->resetFramebuffers();
        }
    }
}

void ResourceManager::makeIrradiancePrefilterMap(const unsigned int cubeMap)
{
    if (m_framebuffers.contains("irradiance"))
    {
        Log.log("Irradiance map already exists... updating instead");
        updateIrradiancePrefilterMap(cubeMap);
        return;
    }

    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
                     GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int captureFBO;
    glGenFramebuffers(1, &captureFBO);

    unsigned int captureRBO;
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    loadModel("assets/defaultAssets/Models/cube.fbx");
    auto mesh = getModel("assets/defaultAssets/Models/cube.fbx").m_meshes[0];
    loadMaterial("assets/defaultAssets/Materials/irradiance.json");
    auto mat = getMaterial("assets/defaultAssets/Materials/irradiance.json");

    auto captureProjection = Matrix4::PerspectiveMatrix(toRadians(90.0f), 1.0f, 0.1f, 10.0f);
    Matrix4 captureViews[] = {
        Matrix4::LookAt(Vector3{}, Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f, 0.0f,  1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  -1.0f,  0.0f), Vector3(0.0f, 0.0f,  -1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  -1.0f), Vector3(0.0f, -1.0f,  0.0f))
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    mat.setUniform("environmentMap", 0);

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    mat.load();

    mat.setUniform("projection", captureProjection);
    for (unsigned int i = 0; i < 6; ++i)
    {
        mat.setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(mesh.VAO());
        glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto framebuffer = FrameBuffer{};
    framebuffer.m_id = captureFBO;
    framebuffer.m_colorBuffer = -1;
    framebuffer.m_renderBuffer = captureRBO;
    auto texture = Texture{};
    texture.m_type = CUBEMAP;
    texture.m_textureID = irradianceMap;

    m_framebuffers.insert({"irradiance", framebuffer});
    m_loadedTextures.insert({"irradiance", texture});

    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    loadMaterial("assets/defaultAssets/Materials/prefilter.json");
    mat = getMaterial("assets/defaultAssets/Materials/prefilter.json");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    mat.setUniform("environmentMap", 0);
    mat.setUniform("projection", captureProjection);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        const auto mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        const auto mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
        mat.setUniform("roughness", roughness);

        for (unsigned int i = 0; i < 6; ++i)
        {
            //prefilterShader.setMat4("view", captureViews[i]);
            mat.setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(mesh.VAO());
            glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
    glBindVertexArray(0);
    activateFramebuffer();
    Window.resetViewport();

    texture = Texture{};
    texture.m_type = CUBEMAP;
    texture.m_textureID = prefilterMap;

    m_loadedTextures.insert({"prefilter", texture});
}

void ResourceManager::updateIrradiancePrefilterMap(unsigned int cubeMap)
{
    auto irradianceMap = m_loadedTextures.at("irradiance").m_textureID;

    auto mesh = getModel("assets/defaultAssets/Models/cube.fbx").m_meshes[0];
    auto mat = getMaterial("assets/defaultAssets/Materials/irradiance.json");

    auto captureProjection = Matrix4::PerspectiveMatrix(toRadians(90.0f), 1.0f, 0.1f, 10.0f);
    Matrix4 captureViews[] = {
        Matrix4::LookAt(Vector3{}, Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f, 0.0f,  1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  -1.0f,  0.0f), Vector3(0.0f, 0.0f,  -1.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
        Matrix4::LookAt(Vector3{}, Vector3(0.0f,  0.0f,  -1.0f), Vector3(0.0f, -1.0f,  0.0f))
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    mat.setUniform("environmentMap", 0);

    glViewport(0, 0, 32, 32);
    activateFramebuffer("irradiance");

    mat.load();

    mat.setUniform("projection", captureProjection);
    for (unsigned int i = 0; i < 6; ++i)
    {
        mat.setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(mesh.VAO());
        glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_INT, nullptr);
    }

    auto prefilterMap = m_loadedTextures.at("prefilter").m_textureID;

    mat = getMaterial("assets/defaultAssets/Materials/prefilterMap.json");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    mat.setUniform("environmentMap", 0);
    mat.setUniform("projection", captureProjection);

    unsigned int captureRBO = m_framebuffers.at("irradiance").m_renderBuffer;

    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // resize framebuffer according to mip-level size.
        const auto mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        const auto mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        const float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
        mat.setUniform("roughness", roughness);

        for (unsigned int i = 0; i < 6; ++i)
        {
            //prefilterShader.setMat4("view", captureViews[i]);
            mat.setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(mesh.VAO());
            glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
    glBindVertexArray(0);
    activateFramebuffer();
    Window.resetViewport();
}

void ResourceManager::loadLights(Material mat)
{
    int lightNumber = 0;
    bool dir = false;
    for (const auto& scene : m_loadedScenes | std::views::values)
    {
        for (const auto& light : scene->m_lightComponents)
        {
            if (light->m_type == DIR)
            {
                if (dir)
                {
                    Log.logWarning("One directional light is already loaded... replacing");
                }
                glUseProgram(mat.m_shaderProgram);
                mat.setUniform("dirLightDir", Vector3(light->m_direction));
                mat.setUniform("dirLightColor", light->m_color);
                mat.setUniform("dirLightPow", light->m_power);
                dir = true;
                continue;
            }
            glUseProgram(mat.m_shaderProgram);
            mat.setUniform("lightPositions[" + std::to_string(lightNumber) + "]", light->getParent()->getWorldPosition());
            mat.setUniform("lightColors[" + std::to_string(lightNumber) + "]", light->m_color);
            mat.setUniform("lightDirections[" + std::to_string(lightNumber) + "]", light->m_direction);
            mat.setUniform("lightPowers[" + std::to_string(lightNumber) + "]", light->m_power);
            mat.setUniform("lightRadius[" + std::to_string(lightNumber) + "]", light->m_power);
            lightNumber++;
        }
    }
}

std::int32_t ResourceManager::convert_to_int(const char* buffer, const std::size_t len)
{
    std::int32_t a = 0;
    if(std::endian::native == std::endian::little)
        std::memcpy(&a, buffer, len);
    else
        for(std::size_t i = 0; i < len; ++i)
            reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
    return a;
}

bool ResourceManager::load_wav_file_header(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample, ALsizei& size)
{
    char buffer[4];
    if(!file.is_open())
        return false;

    // the RIFF
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read RIFF");
        return false;
    }
    if(std::strncmp(buffer, "RIFF", 4) != 0)
    {
        Log.logError("File is not a valid WAVE file (header doesn't begin with RIFF)");
        return false;
    }

    // the size of the file
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read size of file");
        return false;
    }

    // the WAVE
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read WAVE");
        return false;
    }
    if(std::strncmp(buffer, "WAVE", 4) != 0)
    {
        Log.logError("File is not a valid WAVE file (header doesn't contain WAVE)");
        return false;
    }

    // "fmt/0"
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read fmt/0");
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read the 16");
        return false;
    }

    // PCM should be 1?
    if(!file.read(buffer, 2))
    {
        Log.logError("Could not read PCM");
        return false;
    }

    // the number of channels
    if(!file.read(buffer, 2))
    {
        Log.logError("Could not read number of channels");
        return false;
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read sample rate");
        return false;
    }
    sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if(!file.read(buffer, 4))
    {
        Log.logError("Could not read (sampleRate * bitsPerSample * channels) / 8");
        return false;
    }

    // ?? dafaq
    if(!file.read(buffer, 2))
    {
        Log.logError("Could not read dafaq");
        return false;
    }

    // bitsPerSample
    if(!file.read(buffer, 2))
    {
        Log.logError("Could not read bits per sample");
        return false;
    }
    bitsPerSample = convert_to_int(buffer, 2);

    while(!file.eof())
    {
        if(!file.read(buffer, 4))
        {
            Log.logError("Could not read chunk header");
        }

        if (std::strncmp(buffer, "data", 4) != 0)
        {
            if(!file.read(buffer, 4))
            {
                std::cerr << "ERROR: could not read data size" << std::endl;
                return false;
            }
            int chunkSize = convert_to_int(buffer, 4);
            if(chunkSize % 2 != 0)
                chunkSize++;

            if(!file.seekg(chunkSize, std::ios::cur))
            {
                Log.logError("Could not seek past chunk");
                return false;
            }
        }

        if(!file.read(buffer, 4))
        {
            std::cerr << "ERROR: could not read data size" << std::endl;
            return false;
        }
        size = convert_to_int(buffer, 4);

        /* cannot be at the end of file */
        if(file.eof())
        {
            std::cerr << "ERROR: reached EOF on the file" << std::endl;
            return false;
        }
        if(file.fail())
        {
            std::cerr << "ERROR: fail state set on the file" << std::endl;
            return false;
        }

        return true;
    }
    Log.logError("File is not a valid WAVE file (doesn't have 'data' tag)");
    return false;
}

bool ResourceManager::load_wav(const std::string& filename, std::uint8_t& channels, std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample, std::vector<char>& data)
{
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open())
    {
        Log.logError("Could not open %s for reading", filename.c_str());
        return false;
    }
    int size;
    if(!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
    {
        Log.logError("Could not load wav header of %s", filename.c_str());
        return false;
    }

    data.resize(size);

    in.read(data.data(), size);

    return true;
}

void ResourceManager::loadSound(std::string& path)
{
    if (m_loadedSounds.contains(path))
    {
        m_loadedSounds.at(path).listeners++;
        
        return;
    }

    std::uint8_t channels;
    std::int32_t sampleRate;
    std::uint8_t bitsPerSample;
    std::vector<char> soundData{};
    if (!load_wav(path, channels, sampleRate, bitsPerSample, soundData))
    {
        Log.logError("Could not load WAV file, %s", path.c_str());
        return;
    }

    ALuint buffers[NUM_BUFFERS];
    alGenBuffers(NUM_BUFFERS, &buffers[0]);

    ALenum format;
    if(channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if(channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if(channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if(channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        Log.logError("unrecognised wave format");
        return;
    }

    for(std::size_t i = 0; i < NUM_BUFFERS; ++i)
    {
        alBufferData(buffers[i], format, &soundData[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
    }

    auto s = Sound{};
    memcpy(s.m_ID, buffers, sizeof(buffers));
    s.m_format = format;
    s.m_soundData = soundData;
    s.m_sampleRate = sampleRate;

    m_loadedSounds.insert({path, s});
}

Sound ResourceManager::getSound(const std::string& path) const
{
    if (!m_loadedSounds.contains(path))
    {
        Log.logError("%s doesn't exist in loaded sounds", path.c_str());
        return Sound{};
    }
    return m_loadedSounds.at(path);
}

void ResourceManager::unloadSound(const std::string& path)
{
    if (!m_loadedSounds.contains(path))
    {
        Log.logError("Could not unload sound %s", path.c_str());
        return;
    }
    m_loadedSounds.at(path).listeners--;
    if (m_loadedSounds.at(path).listeners <= 0)
    {
        m_loadedSounds.erase(path);
    }
}

void ResourceManager::unloadFont(const std::string& path)
{
    if (!m_loadedFonts.contains(path))
    {
        Log.logError("Could not unload font %s", path.c_str());
        return;
    }
    if (m_loadedFonts.at(path).listeners == 0)
    {
        m_loadedFonts.erase(path);
    }
}

void ResourceManager::loadFont(const std::string& path)
{
    if (m_loadedFonts.contains(path))
    {
        Log.log("%s already loaded...", path.c_str());
        m_loadedFonts.at(path).listeners++;
        return;
    }
    std::map<char, Character> characters{};
    FT_Face face{};
    FT_Library ft{};

    if (FT_Init_FreeType(&ft))
    {
        Log.logError("Could not init FreeType Library");
        return;
    }

    if (FT_New_Face(ft, path.c_str(), 0, &face))
    {
        Log.logError("Could not load font");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    unsigned int textureArray{};

    glGenTextures(1, &textureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Log.logError("Freetype: Failed to load Glyph");
            continue;
        }
        // generate texture
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, static_cast<int>(c),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            static_cast<int>(c),
            Vector2(static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)),
            Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert({c, character});
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    auto font = Font{};
    font.m_textureArray = textureArray;
    font.m_characters = characters;

    m_loadedFonts.insert({path, font});
}

Font ResourceManager::getFont(const std::string& path) const
{
    if (!m_loadedFonts.contains(path))
    {
        Log.logError("%s is not a loaded font", path.c_str());
        return Font{};
    }
    return m_loadedFonts.at(path);
}

void ResourceManager::makeShadowMap(LightComponent* light)
{
    if (light->getLightType() != DIR)
    {
        return;
    }

    
}

void ResourceManager::addToDraw(const std::function<void()>& function, const bool transparent)
{
    for (auto scene : m_loadedScenes | std::views::values)
    {
        scene->addToDraw(function, transparent);
    }
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
    registerComponent<LightComponent>("Light");
    registerComponent<AudioSourceComponent>("AudioSource");
    registerComponent<TextRendererComponent>("TextRenderer");
    registerComponent<ParticleSystemComponent>("ParticleSystem");

    loadUniformMap();
    makePostprocessingScreen();
    makeBRDFMap();
    loadMaterial("assets/defaultAssets/Materials/shadowMap.json");

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

    auto model = m_loadedModels.at(path);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        Vector3 vector;
        vector[0] = mesh->mVertices[i].x;
        vector[1] = mesh->mVertices[i].y;
        vector[2] = mesh->mVertices[i].z;
        vertex.m_position = vector;

        model.m_boundingBox.min[0] = std::min(model.m_boundingBox.min[0], vector.x());
        model.m_boundingBox.min[1] = std::min(model.m_boundingBox.min[1], vector.y());
        model.m_boundingBox.min[2] = std::min(model.m_boundingBox.min[2], vector.z());

        model.m_boundingBox.max[0] = std::max(model.m_boundingBox.max[0], vector.x());
        model.m_boundingBox.max[1] = std::max(model.m_boundingBox.max[1], vector.y());
        model.m_boundingBox.max[2] = std::max(model.m_boundingBox.max[2], vector.z());

        vector[0] = mesh->mNormals[i].x;
        vector[1] = mesh->mNormals[i].y;
        vector[2] = mesh->mNormals[i].z;
        vertex.m_normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vector2 vec;
            vec[0] = mesh->mTextureCoords[0][i].x;
            vec[1] = mesh->mTextureCoords[0][i].y;
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

    model.m_meshes.push_back(map_mesh);
    m_loadedModels.at(path) = model;
}

void ResourceManager::loadUniformMap()
{
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
        glUniform2f(uniform, value.x(), value.y());
    }});

    m_uniformMap.insert({"Vector3", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = Vector3::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform3f(uniform, value.x(), value.y(), value.z());
    }});

    m_uniformMap.insert({"Vector4", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        const auto value = Vector4::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform4f(uniform, value.x(), value.y(), value.z(), value.w());
    }});

    m_uniformMap.insert({"Matrix4", [](const unsigned int shaderProgram, const nlohmann::json& j)
    {
        const auto name = j["Name"].get<std::string>();
        auto value = Matrix4::fromJson(j["Value"]);

        glUseProgram(shaderProgram);
        const int uniform = glGetUniformLocation(shaderProgram, name.c_str());
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
    }});
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
}

void ResourceManager::makeBRDFMap()
{
    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log.logError("Framebuffer not complete: %d", fboStatus);

    glViewport(0, 0, 512, 512);

    auto mesh = m_loadedModels.at("post").m_meshes.at(0);
    Resource.loadMaterial("assets/defaultAssets/Materials/brdf.json");
    auto mat = Resource.getMaterial("assets/defaultAssets/Materials/brdf.json");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat.load();
    mat.setUniform("model", Matrix4{});

    glBindVertexArray(mesh.VAO());
    glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    activateFramebuffer();

    auto texture = Texture{};
    texture.m_textureID = brdfLUTTexture;

    m_loadedTextures.insert({"brdf", texture});
}