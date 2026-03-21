#include "GameManager.h"
#include "WorldManager.h"
#include "WindowManager.h"
#include "GUI-Classes/GuiObject.h"

int main() {
    Game.initialize();
    World.getActiveScene()->loadDefaultSkybox();

    auto* a = new GuiObject(vec2(0.0f,0.0f), vec2(0.5f,1.0f));

    Game.run();
    Game.terminate();
}
