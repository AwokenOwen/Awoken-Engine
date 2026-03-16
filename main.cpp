#include "GameManager.h"
#include "WorldManager.h"

int main() {
    Game.initialize();

    World.getActiveScene()->loadDefaultSkybox();

    Game.run();
    Game.terminate();
}
