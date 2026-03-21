//
// Created by awokenowen on 3/21/26.
//
#pragma once
#include "../Object.h"

class GuiObject : public Object{

public:
    GuiObject();
    GuiObject(vec2 pos, vec2 size);
private:
    void init(vec2 pos, vec2 size);
};