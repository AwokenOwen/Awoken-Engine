#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "PointLight.h"
#include "test_classes/FPSDisplay.h"
#include "test_classes/MovementComponent.h"

int main(const int argc, char* argv[]) {
    // || Game Manager Init || //
        // Sets up all other managers and prepared the engine to run //
    Game.initialize();

    // || Camera Movement || //
        // Adding a custom movement component onto the camera to move around the scene //
    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();

    // || Lights || //
        // Skybox/IBL ambient lighting //
    //Game.getActiveScene()->loadDefaultSkybox();
        // Directional Light //
    //new DirectionalLight;
        // Single Point light //
            // Point Light init with color and power as inputs //
    const auto light = new PointLight(vec3(1,1,1), 1000);
            // Setting point light position
    light->setLocalPosition(vec3(0,15,0));

    // || FPS DISPLAY || //
    new FPSDisplay();

    // || Ground Material Creation || //
        // Material init //
    const auto terracottaMaterial = new Material();
        // Shader loader custom non default shader using the default shaders as a base //
    terracottaMaterial->setShaderProgram("assets/Shaders/tiledGround.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
        // Shader texture loading the relevant textures for the material //
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_BaseColor.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Metallic.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Roughness.png");
        // Setting the "custom" tile multiplier //
    terracottaMaterial->setUniform<float>("tile", 10.0f);

    // || Object Creation || //
        // Object init //
    const auto a = new Object;
        // Setting position of object //
    a->setLocalPosition(vec3(0.0,-1.0,0.0));
        // Adding the MeshRenderer component and loading the default plane model //
    a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/plane.fbx");
        // Setting the material (of material position 0) to the material made above //
    a->getComponent<MeshRenderer>()->setMaterial(terracottaMaterial);

    // || Game Manager Run || //
        // Begins the game loop and continues until called to quit //
    Game.run();
    // || Game Manager Terminate || //
        // Call terminate on all Managers freeing all data needed to be freed then closes game//
    Game.terminate();
}
