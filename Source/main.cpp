//
// Created by awokenowen on 4/12/26.
//

#include "GameManager.h"
#include "Object.h"
#include "WindowManager.h"
#include "WorldManager.h"

int main(int argc, char *argv[]) {
    Window.setWindowTitle("Awoken-Engine");
    Game.initialize();

    /*std::ofstream outFile("jsonTest.txt");

    new Object();

    outFile << std::setw(4) << World.getActiveScene()->toJson();

    outFile.close();*/

    //World.loadScene("assets/Scenes/jsonTest.txt");

    World.setActiveScene("assets/Scenes/jsonTest.txt", true);

    Game.run();
    Game.terminate();
}
