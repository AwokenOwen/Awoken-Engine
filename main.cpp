#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "test_classes/MovementComponent.h"
#include "test_classes/FPSDisplay.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    new DirectionalLight;

    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();

    new FPSDisplay;

    // IBL Testing
    const auto sphere = new Object;
    sphere->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    sphere->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
        "assets/Shaders/smooth_sphere.vert",
        "assets/Shaders/default_IBL.frag"
    );
    sphere->setLocalPosition(vec3(0,0,5));


    Game.run();
    Game.terminate();
}
