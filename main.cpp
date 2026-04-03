#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "test_classes/FPSDisplay.h"
#include "test_classes/MovementComponent.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();

    new DirectionalLight;

    new FPSDisplay;

    const auto goldMaterial = new Material();
    goldMaterial->setShaderProgram("assets/Shaders/smooth_sphere.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_BaseColor.png");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Metallic.png");
    goldMaterial->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Roughness.png");

    const auto a = new Object;
    a->setLocalPosition(vec3(0.0,0.0,5.0));
    a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    a->getComponent<MeshRenderer>()->setMaterial(goldMaterial);

    const auto barkMaterial = new Material();
    barkMaterial->setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/Shaders/Textured_Albeto_Normal_Metalic_Roughness.frag");
    barkMaterial->addTexture("assets/Textures/WoodBark/sphere_DefaultMaterial_BaseColor.png");
    barkMaterial->addTexture("assets/Textures/WoodBark/sphere_DefaultMaterial_Normal.png");
    barkMaterial->addTexture("assets/Textures/WoodBark/sphere_DefaultMaterial_Metallic.png");
    barkMaterial->addTexture("assets/Textures/WoodBark/sphere_DefaultMaterial_Roughness.png");

    const auto b = new Object;
    b->setLocalPosition(vec3(-2.0,0.0,5.0));
    b->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    b->getComponent<MeshRenderer>()->setMaterial(barkMaterial);

    Game.run();
    Game.terminate();
}
