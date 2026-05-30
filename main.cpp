//
// Created by AwokenOwen on 5/12/26.
//
#include <fstream>

#include "GameManager.h"
#include "Object.h"
#include "ResourceManager.h"

int main(int argc, char* argv[])
{
    // Make the scenes for the game before running
    // Make a name
    // All the objects with their components turned into JSON

    /*auto scene = new Scene("testScene");

    auto image = new Object();
    image->addComponent<ModelRendererComponent>();



    scene->m_rootObjects.push_back(image);
    std::ofstream f("assets/defaultAssets/Scenes/testScene.scene");
    f << scene->toJson().dump(4)  << std::endl;

    delete image;
    delete scene;*/

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

    nlohmann::json material;
    material["VertexShader"] = "assets/defaultAssets/Shaders/unlit.vert";
    material["FragmentShader"] = "assets/defaultAssets/Shaders/unlit.frag";

    std::vector<nlohmann::json> uniforms;
    nlohmann::json uniform;
    uniform["Type"] = "Vector3";
    uniform["Name"] = "color";

    nlohmann::json colorvector;

    colorvector["x"] = 1.0f;
    colorvector["y"] = 1.0f;
    colorvector["z"] = 1.0f;

    uniform["Value"] = colorvector;

    uniforms.push_back(uniform);

    material["Uniforms"] = uniforms;

    std::ofstream unlit("assets/defaultAssets/Materials/unlit.json");
    unlit << material.dump(4)  << std::endl;


    /*
    // Here all the scenes will be added to a standby map and only the Primary will become the base scene
    Game.initialize();

    // All custom components must be registered here, after initialize but before run
    // call Resource.registerComponent<COMPONENT_TYPE>("THE NAME IN THE SCENE FILE")

    // The starting base scene will be loaded with all the objects and components
    Game.run();
    // All heap data will be deleted (not that is matters since this is the end of the program)
    Game.terminate();*/
}
