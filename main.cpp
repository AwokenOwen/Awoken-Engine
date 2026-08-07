//
// Created by AwokenOwen on 5/12/26.
//
#include "GameManager.h"

int main(int argc, char* argv[])
{
     // Here all the scenes will be added to a standby map and only the Primary will become the base scene
     Game.initialize();

     // All custom components must be registered here, after initialize but before run
     // call Resource.registerComponent<COMPONENT_TYPE>("THE NAME IN THE SCENE FILE")

     // The starting base scene will be loaded with all the objects and components
     Game.run();
     // All heap data will be deleted (not that is matters since this is the end of the program)
     Game.terminate();
}
