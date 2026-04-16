#include <iostream>

#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "ResourceManager.h"
#include "test_classes/MovementComponent.h"

int main(const int argc, char* argv[]) {
    Game.initialize();

    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();
    Game.getActiveScene()->getCamera()->setLocalPosition(vec3(0.0,2.0,0.0));

    //Game.getActiveScene()->loadDefaultSkybox();
    //new DirectionalLight;

    const auto terracottaMaterial = new Material();
    terracottaMaterial->setShaderProgram("assets/Shaders/tiledGround.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_BaseColor.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Metallic.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Roughness.png");
    terracottaMaterial->setUniform<float>("tile", 10.0f);

    const auto goldMaterial = new Material();
    goldMaterial->setShaderProgram("assets/Shaders/smooth_sphere.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_BaseColor.png");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Metallic.png");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Roughness.png");

    const auto a = new Object;
    a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/plane.fbx");
    a->getComponent<MeshRenderer>()->setMaterial(terracottaMaterial);
    a->setLocalPosition(vec3(0,0,60));

    const auto sphere = new Object;
    sphere->setLocalPosition(vec3(0,1,5));
    sphere->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    sphere->getComponent<MeshRenderer>()->setMaterial(goldMaterial);



    for (auto i = 0; i < 7; ++i) {
        auto light = new PointLight(vec3(1,1,1), 20);
        light->setLocalPosition(vec3(1.43,4.19,0));

        auto b = new Object;
        b->setLocalPosition(vec3(-5.0,0.0,i * 15.0));
        b->addComponent<MeshRenderer>()->loadModel("assets/Models/LightPost.fbx");
        b->addChild(light);
        b->getComponent<MeshRenderer>()->getMaterials()[1]->setShaderProgram("assets/Shaders/smooth_sphere.vert", "assets/Shaders/EmissiveOnly.frag");

        auto light2 = new PointLight(vec3(1,1,1), 20);
        light2->setLocalPosition(vec3(1.43,4.19,0));

        auto b2 = new Object;
        b2->setLocalPosition(vec3(5.0,0.0,i * 15.0));
        b2->setLocalRotation(vec3(0.0,glm::radians(180.0),0.0));
        b2->addComponent<MeshRenderer>()->loadModel("assets/Models/LightPost.fbx");
        b2->addChild(light2);
        b2->getComponent<MeshRenderer>()->getMaterials()[1]->setShaderProgram("assets/Shaders/smooth_sphere.vert", "assets/Shaders/EmissiveOnly.frag");
    }

    Game.run();
    Game.terminate();
}
