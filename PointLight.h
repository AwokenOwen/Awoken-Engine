#pragma once
#include "glm.hpp"
#include "Object.h"

using namespace glm;

class PointLight : public Object
{
public:
    PointLight(vec3 color = vec3(1.0f, 1.0f, 1.0f), float power = 1.0f);

    vec3 getColor();
    float getPower();

    void setColor(vec3 color);
    void setPower(float power);

private:
    vec3 color;
    float power;
};

