#include "GameManager.h"
#include "Scene.h"
#include "Object.h"
#include "TextRenderer.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    auto a = new Object();
    a->addComponent<TextRenderer>();

    Game.run();
    Game.terminate();
}
