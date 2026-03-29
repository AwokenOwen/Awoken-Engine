#include "GameManager.h"
#include "Scene.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    Game.run();
    Game.terminate();
}
