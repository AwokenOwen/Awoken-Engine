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

    const auto terracottaMaterial = new Material();
    terracottaMaterial->setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_BaseColor.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Metallic.png");
    terracottaMaterial->addTexture("assets/Textures/Terracotta/sphere_DefaultMaterial_Roughness.png");

    const auto a = new Object;
    a->setLocalPosition(vec3(0.0,-1.0,0.0));
    a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/plane.fbx");
    a->getComponent<MeshRenderer>()->setMaterial(terracottaMaterial);

    Game.run();
    Game.terminate();
}
