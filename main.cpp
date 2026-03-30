#include <iostream>

#include "GameManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Object.h"
#include "TextRenderer.h"
#include "AudioManager.h"
#include "AudioSource.h"
#include "MeshRenderer.h"

int main(const int argc, char* argv[]) {
    Game.initialize();
    Game.getActiveScene()->loadDefaultSkybox();

    const auto a = new Object();
    a->addComponent<TextRenderer>()->setText("BITCH");
    a->getComponent<TextRenderer>()->setColor(vec3(0.2f, 0.7f, 0.2f));

    a->setLocalScale(vec3(5, 5, 1));

    const auto b = new Object();

    Audio.setListener(b);

    b->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
    b->setLocalPosition(vec3(0, 0, 5));

    Game.run();
    Game.terminate();
}
