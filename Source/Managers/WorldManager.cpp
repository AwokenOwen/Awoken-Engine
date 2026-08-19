//
// Created by AwokenOwen on 4/13/26.
//

#include "WorldManager.h"
#include "Object.h"

#include "CameraComponent.h"
#include "LightComponent.h"
#include "ResourceManager.h"
#include "WindowManager.h"

WorldManager & WorldManager::getInstance() {
    // Make singleton
    static WorldManager instance;
    // return singleton
    return instance;
}

void WorldManager::setMainCamera(CameraComponent* camera)
{
    if (m_mainCamera != nullptr)
    {
        m_mainCamera->m_main = false;
    }
    m_mainCamera = camera;
    m_mainCamera->m_main = true;
}

CameraComponent* WorldManager::getMainCamera() const
{
    return m_mainCamera;
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

    Resource.LoadBaseScene();

    exit(0);

    Resource.awake();
}

void WorldManager::update() const
{
    for (const auto& scene : Resource.m_loadedScenes | std::views::values)
    {
        // Load Step
        scene->LoadEvent.call();
        scene->LoadEvent.clear();

        // Update Step
        scene->UpdateEvent.call();

        // Physics Step

        // Draw Step
        for (const auto camera : scene->m_cameraComponents)
        {
            camera->drawToFrameBuffer();
        }

        drawPostprocess();

        // Unload Step
        scene->UnloadEvent.call();
        scene->UnloadEvent.clear();

        // Destroy Step
        scene->DestroyEvent.call();
        scene->DestroyEvent.clear();
    }
}

void WorldManager::drawPostprocess() const
{
    const auto screen = Resource.getModel("post").m_meshes[0];
    auto mat = Resource.getMaterial("assets/defaultAssets/Materials/postprocess.json");

    mat.load();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_mainCamera->m_intermediateFrameBuffer.m_colorBuffer);

    glBindVertexArray(screen.VAO());
    glDrawElements(GL_TRIANGLES, screen.indexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
