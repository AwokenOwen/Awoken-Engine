#include "PointLight.h"
#include "WorldManager.h"

PointLight::PointLight(vec3 color, float power) : Object()
{
	this->color = color;
	this->power = power;

	World.getActiveScene()->pointLights.push_back(this);
}

vec3 PointLight::getColor() 
{
	return color;
}

float PointLight::getPower() 
{
	return power;
}

void PointLight::setColor(vec3 color)
{
	this->color = color;
}

void PointLight::setPower(float power)
{
	this->power = power;
}