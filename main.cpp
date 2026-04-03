#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "test_classes/MovementComponent.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();

    const auto a = new Object;
    a->setLocalPosition(vec3(0.0,0.0,5.0));
    a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    a->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram("assets/Shaders/smooth_sphere.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");
    a->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_BaseColor.png");
    a->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Metallic.png");
    a->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/Gold/sphere_DefaultMaterial_Roughness.png");


    Game.run();
    Game.terminate();
}
