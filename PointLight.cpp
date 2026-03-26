#include "PointLight.h"
#include "WorldManager.h"

PointLight::PointLight(const vec3 color, const float power) : Object()
{
	m_color = color;
	m_power = power;

	World.getActiveScene()->pointLights.push_back(this);
}

vec3 PointLight::getColor() const {
	return m_color;
}

float PointLight::getPower() const {
	return m_power;
}

void PointLight::setColor(const vec3 color)
{
	m_color = color;
}

void PointLight::setPower(const float power)
{
	m_power = power;
}