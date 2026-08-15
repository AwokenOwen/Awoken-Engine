//
// Created by awokenowen on 4/24/26.
//

#include "ModelRendererComponent.h"

#include <iostream>

#include "CameraComponent.h"
#include "GameManager.h"
#include "LightComponent.h"
#include "LogManager.h"
#include "Object.h"


void ModelRendererComponent::update()
{
    // Check if mesh count is equal to the material count
    if (m_model.meshCount() != m_materials.size())
        // Log error if not
        Log.logError("Mesh Count does not match Material Count.");
    // Add each mesh's draw call to the loaded camera
    for (int i = 0; i < m_model.meshCount(); ++i)
    {
        // Call the add to draw function and make a lambda function to push through
        Resource.addToDraw([this, i]()
        {
            // Load the default uniforms
            this->defaultDynamicUniformLoader(m_materials[i]);

            // Make the draw call
            glBindVertexArray(m_model.m_meshes[i].VAO());
            glDrawElements(GL_TRIANGLES, m_model.m_meshes[i].indexCount(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }, false);
    }
}

nlohmann::json ModelRendererComponent::toJson()
{
    // Create the JSON object
    nlohmann::json j;

    // Push the type
    j["Type"] = "ModelRenderer";

    // Push the model path
    j["Model"] = m_modelPath;

    // Push the material paths
    j["Materials"] = m_materialPaths;

    // Return the json
    return j;
}

void ModelRendererComponent::fromJson(nlohmann::json j)
{
    // Set the model path
    m_modelPath = j["Model"].get<std::string>();

    // Set the material paths
    m_materialPaths = j["Materials"].get<std::vector<std::string>>();

    // Load and get the model
    Resource.loadModel(m_modelPath);
    m_model = Resource.getModel(m_modelPath);

    // Load and get all the materials
    for (const auto& m : m_materialPaths)
    {
        Resource.loadMaterial(m);
        m_materials.emplace_back(Resource.getMaterial(m));
    }
}

void ModelRendererComponent::defaultDynamicUniformLoader(Material mat) const
{
    // Enable cull faces
    glEnable(GL_CULL_FACE);
    // Set the cull face to the back face
    glCullFace(GL_BACK);

    // Load the material
    mat.load();

    // Get the matrices
    const auto model = getParent()->getWorldMatrix();
    const auto view = World.getMainCamera()->getViewMatrix();
    const auto proj = World.getMainCamera()->getProjectionMatrix();
    const auto orthographic = World.getMainCamera()->getOrthographicMatrix();

    switch (mat.getType())
    {
        // For more custom materials
    case NONE:
        break;
    case UNLIT:
        // Set the model view and projection matrices
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        // Use proj so the camera decides which matrix to use
        mat.setUniform("projection", proj);
        break;
    case DEFAULT_LIT:
        // Set the model view and projection matrices
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        // Use proj so the camera decides which matrix to use
        mat.setUniform("projection", proj);

        // Load the lights into the material
        Resource.loadLights(mat);
        break;
    case DEFAULT_SPRITE:
        // Set the model view and projection matrices
        mat.setUniform("model", model);
        mat.setUniform("view", view);
        // 2D sprites get the orthographic matrix
        mat.setUniform("projection", orthographic);
        break;
    }
}

void ModelRendererComponent::destroy()
{
    // Tell the Resource Manager this component no longer needs this model
    Resource.unloadModel(m_modelPath);

    // Tell the Resource Manager this component no longer needs these materials
    for (const auto& m : m_materialPaths)
    {
        Resource.unloadMaterial(m);
    }
}
