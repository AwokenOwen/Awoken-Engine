#include "GameManager.h"
#include "WorldManager.h"


int main(const int argc, char* argv[]) {
    Game.initialize();
    World.getActiveScene()->loadDefaultSkybox();

    Game.run();
    Game.terminate();
}
