#include "Mesh.h"
#include "Material.h"
#include "glad/glad.h"
#include "WorldManager.h"
#include "WindowManager.h"
#include "Camera.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <gtc/type_ptr.hpp>
#include "MeshRenderer.h"
#include "GameManager.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    once = true;
    materialOwner = false;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Mesh::Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::setUpShaderMatrices(unsigned int shaderProgram)
{
    // Vertex Matrix Stuff
    mat4 modelMatrix = getParent()->worldModelMatrix();
    mat4 projectionMatrix = Window.getPerspectiveMatrix();
    mat4 viewMatrix = World.getActiveScene()->getCamera()->getViewMatrix();

    if (material->type == MaterialType::CUBEMAP)
    {
        viewMatrix = glm::mat4(glm::mat3(viewMatrix));
    }

    if (material->type == MaterialType::UI)
    {
        projectionMatrix = Window.getOrthographicMatrix();
        vec3 UIPos = getParent()->GetWorldPosition() * Window.getPixelRatio();

        switch (material->anchorPoint)
        {
        case UIAnchorPoints::TOP:
            UIPos.y += static_cast<float>(Window.getHeight()) / 2.0f;
            break;
        case UIAnchorPoints::BOTTOM:
            UIPos.y += -static_cast<float>(Window.getHeight()) / 2.0f;
            break;
        case UIAnchorPoints::RIGHT:
            UIPos.x += static_cast<float>(Window.getWidth()) / 2.0f;
            break;
        case UIAnchorPoints::LEFT:
            UIPos.x += -static_cast<float>(Window.getWidth()) / 2.0f;
            break;
        case UIAnchorPoints::TOP_RIGHT:
            UIPos.x += static_cast<float>(Window.getWidth()) / 2.0f;
            UIPos.y += static_cast<float>(Window.getHeight()) / 2.0f;
            break;
        case UIAnchorPoints::BOTTOM_RIGHT:
            UIPos.x += static_cast<float>(Window.getWidth()) / 2.0f;
            UIPos.y += -static_cast<float>(Window.getHeight()) / 2.0f;
            break;
        case UIAnchorPoints::TOP_LEFT:
            UIPos.y += static_cast<float>(Window.getHeight()) / 2.0f;
            UIPos.x += -static_cast<float>(Window.getWidth()) / 2.0f;
            break;
        case UIAnchorPoints::BOTTOM_LEFT:
            UIPos.y += -static_cast<float>(Window.getHeight()) / 2.0f;
            UIPos.x += -static_cast<float>(Window.getWidth()) / 2.0f;
            break;
        default:
            break;
        }

        UIPos = vec3(2.0f * UIPos.x / static_cast<float>(Window.getHeight()), 2.0f * UIPos.y / static_cast<float>(Window.getHeight()), 0);

        vec3 rotationVec = eulerAngles(getParent()->GetWorldRotation());
        rotationVec.z = 0;
        quat rotation = quat(rotationVec);

        mat4 transformMatrix = translate(mat4(1.0f), UIPos);
        mat4 rotationMatrix = rotate(mat4(1.0f), angle(rotation), axis(rotation));
        mat4 scaleMatrix = scale(mat4(1.0), parent->GetWorldScale() / 500.0f);

        modelMatrix = transformMatrix * rotationMatrix * scaleMatrix;
    }

    material->setUniform<mat4>("model", modelMatrix);

    //int modelLoc = glGetUniformLocation(shaderProgram, "model");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Mesh::setUpShaderVariables(unsigned int shaderProgram)
{
    // Ambient Light
    vec3 ambientColor = World.getActiveScene()->ambientColor;

    int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
    glUniform3f(ambientColorLoc, ambientColor.x, ambientColor.y, ambientColor.z);

    float ambientPower = World.getActiveScene()->ambientPower;

    int ambienPowertLoc = glGetUniformLocation(shaderProgram, "ambientPower");
    glUniform1f(ambienPowertLoc, ambientPower);

    vec3 cameraPosition = World.getActiveScene()->getCamera()->GetWorldPosition();

    int cameraLoc = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3f(cameraLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

void Mesh::setUpDirectionalLight(unsigned int shaderProgram)
{
    // Directional Light
    if (World.getActiveScene()->directionalLight != nullptr)
    {
        vec3 direction = -World.getActiveScene()->directionalLight->getDirection();
        float power = World.getActiveScene()->directionalLight->getPower();
        vec3 color = World.getActiveScene()->directionalLight->getColor();

        int directionLoc = glGetUniformLocation(shaderProgram, "dirLightDir");
        glUniform3f(directionLoc, direction.x, direction.y, direction.z);

        int powerLoc = glGetUniformLocation(shaderProgram, "dirLightPow");
        glUniform1f(powerLoc, power);

        int colorLoc = glGetUniformLocation(shaderProgram, "dirLightColor");
        glUniform3f(colorLoc, color.x, color.y, color.z);
    }
}

void Mesh::setUpPointLights(unsigned int shaderProgram)
{
    for (int i = 0; i < World.getActiveScene()->pointLights.size(); i++)
    {
        // Point Lights
        vec3 position = World.getActiveScene()->pointLights[i]->GetWorldPosition();
        float power = World.getActiveScene()->pointLights[i]->getPower();
        vec3 color = World.getActiveScene()->pointLights[i]->getColor();

        string command = string("lightPositions[") + to_string(i).c_str() + "]";
        int positionLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform3f(positionLoc, position.x, position.y, position.z);

        command = string("lightPowers[") + to_string(i).c_str() + "]";
        int powerLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform1f(powerLoc, power);

        command = string("lightColors[") + to_string(i).c_str() + "]";
        int colorLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform3f(colorLoc, color.x, color.y, color.z);
    }
}

void Mesh::Draw()
{
    // draw mesh
    if (!material->twoSided)
    {
        glEnable(GL_CULL_FACE);

        if (material->type == MaterialType::CUBEMAP)
        {
            glCullFace(GL_FRONT);
        }
        else {
            glCullFace(GL_BACK);
        }
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    unsigned int shaderProgram = material->getShaderProgram();

    glUseProgram(shaderProgram);

    // Load Textures
    material->loadTextures();

    material->setUniform<vec3>("objectPos", parent->GetLocalPosition());
    material->setUniform<float>("time", Game.getTime());

    setUpShaderMatrices(shaderProgram);
    setUpShaderVariables(shaderProgram);
    setUpDirectionalLight(shaderProgram);
    setUpPointLights(shaderProgram);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


Object* Mesh::getParent() 
{
    return parent;
}

void Mesh::setParent(Object* parent) 
{
    this->parent = parent;
}