//
// Created by AwokenOwen on 5/12/26.
//

#include <fstream>
#include <nlohmann/json.hpp>

#include "GameManager.h"
#include "Object.h"
#include "ResourceManager.h"
#include "CameraComponent.h"

int main(int argc, char* argv[])
{
    // This will run any game we give it -> start with a gameInit.json file,
    // This loads the scenes into the engine
    // Primary -> is the starting scene
    // Scenes -> is a list of all the other scenes in the game
    auto scene = new Scene("cameraScene");
    auto cameraObject = new Object();

    cameraObject->addComponent<CameraComponent>();
    scene->m_rootObjects.push_back(cameraObject);

    std::ofstream f("assets/defaultAssets/Scenes/cameraScene.scene");
    f << scene->toJson().dump(4) << std::endl;

    Game.initialize();
    Game.run();
    Game.terminate();

    delete cameraObject;
    delete scene;
}
