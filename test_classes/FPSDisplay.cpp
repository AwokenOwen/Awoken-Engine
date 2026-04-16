#include "FPSDisplay.h"

#include "../TextRenderer.h"
#include "../GameManager.h"
#include "../Material.h"

FPSDisplay::FPSDisplay(){
    addComponent<TextRenderer>()->getMaterial()->setTextAnchorPoint(TOP_LEFT);
    getComponent<TextRenderer>()->getMaterial()->setScreenAnchorPoint(TOP_LEFT);
    getComponent<TextRenderer>()->setColor(vec3(1.0f, 1.0f, 1.0f));
}

void FPSDisplay::update() {
    string fps = "FPS: " + to_string(static_cast<int>(1.0 / Game.getDeltaTime()));
    getComponent<TextRenderer>()->setText(fps);

    Object::update();
}
