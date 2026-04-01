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

    Game.run();
    Game.terminate();
}
