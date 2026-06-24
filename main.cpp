//
// Created by AwokenOwen on 5/12/26.
//
#include <fstream>

#include "AudioSourceComponent.h"
#include "GameManager.h"
#include "ModelRendererComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "Object.h"
#include "ResourceManager.h"

int main(int argc, char* argv[])
{
    // Make the scenes for the game before running
    // Make a name
    // All the objects with their components turned into JSON

    auto scene = new Scene("testScene");

    auto dir = new Object();
    dir->addComponent<LightComponent>();

    auto cube = new Object();
    cube->addComponent<ModelRendererComponent>();
    cube->addComponent<AudioSourceComponent>();

    cube->setLocalPosition(Vector3(0, 0, -1000));
    cube->setLocalScale(Vector3(100, 100, 100));
    cube->setLocalRotation(Quaternion(Vector3(45, 45, 0)));

    auto floor = new Object();
    floor->addComponent<ModelRendererComponent>();

    floor->setLocalPosition(Vector3(0, -200, 0));
    floor->setLocalScale(Vector3(10000, 1, 10000));

    auto camera = new Object();
    camera->addComponent<CameraComponent>();

    camera->setLocalPosition(Vector3(0, 0, 0));

    scene->m_rootObjects.push_back(dir);
    scene->m_rootObjects.push_back(cube);
    scene->m_rootObjects.push_back(floor);
    scene->m_rootObjects.push_back(camera);
    std::ofstream f("assets/defaultAssets/Scenes/testScene.scene");
    f << scene->toJson().dump(4)  << std::endl;

    delete dir;
    delete cube;
    delete floor;
    delete scene;
    delete camera;

    // Then Alter gameInit.json to load the scene
    // Primary is the first scene that will be loaded at start
    // Scenes is a list of all the scenes on stand by
    // Models is a list of all the paths to models in the game

    /*nlohmann::json j;
    j["Primary"] = "assets/defaultAssets/Scenes/testScene.scene";
    j["Scenes"] = std::vector<std::string>({ "assets/defaultAssets/Scenes/default.scene" });

    std::ofstream gameInit("gameInit.json");
    gameInit << j.dump(4)  << std::endl;*/


    // Material making

    /*nlohmann::json material;
    material["VertexShader"] = "assets/defaultAssets/Shaders/unlit.vert";
    material["FragmentShader"] = "assets/defaultAssets/Shaders/unlit.frag";

    std::vector<nlohmann::json> uniforms;
    nlohmann::json uniform;
    uniform["Type"] = "Vector3";
    uniform["Name"] = "color";

    uniform["Value"] = Vector3(1.0f, 0.0f, 1.0f).toJson();

    uniforms.push_back(uniform);

    material["Uniforms"] = uniforms;

    material["Textures"] = std::vector<std::string>();

    std::ofstream unlit("assets/defaultAssets/Materials/unlit.json");
    unlit << material.dump(4)  << std::endl;*/


     // Here all the scenes will be added to a standby map and only the Primary will become the base scene
     Game.initialize();

     // All custom components must be registered here, after initialize but before run
     // call Resource.registerComponent<COMPONENT_TYPE>("THE NAME IN THE SCENE FILE")

     // The starting base scene will be loaded with all the objects and components
     Game.run();
     // All heap data will be deleted (not that is matters since this is the end of the program)
     Game.terminate();
}
