#include "GameManager.h"
#include "Scene.h"
#include "Object.h"
#include "TextRenderer.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    const auto a = new Object();
    a->addComponent<TextRenderer>();

    a->setLocalScale(vec3(5, 5, 1));

    Game.run();
    Game.terminate();
}
