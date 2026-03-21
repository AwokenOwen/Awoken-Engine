//
// Created by awokenowen on 3/21/26.
//

#include "GuiObject.h"

#include "Material.h"
#include "MeshRenderer.h"

GuiObject::GuiObject() : Object(){
    init(vec2(0,0), vec2(1,1));
}

GuiObject::GuiObject(const vec2 pos, const vec2 size) : Object(){
    init(pos, size);
}

void GuiObject::init(const vec2 pos, const vec2 size) {
    localPosition = vec3(pos, 1.0);
    localScale  = vec3(size, 1.0);

    auto* m = addComponent<MeshRenderer>();
    m->loadModel("assets/defaultAssets/Models/image.fbx");
    m->material->type = MaterialType::UI;
    m->material->setShaderProgram("assets/defaultAssets/Shaders/defaultGUI.vert", "assets/defaultAssets/Shaders/defaultGUI.frag");
}