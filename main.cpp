#include "GameManager.h"
#include "WorldManager.h"
#include "WindowManager.h"

int main() {
    Game.initialize();
    World.getActiveScene()->loadDefaultSkybox();

    Game.run();
    Game.terminate();
}
