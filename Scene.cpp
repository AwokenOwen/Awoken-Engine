#include "Scene.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Camera.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "MeshRenderer.h"
#include "Material.h"
#include <algorithm>

Scene::Scene()
{
	m_ambientColor = vec3(1.0f);
	m_ambientPower = 0.1f;
	m_directionalLight = nullptr;
}

void Scene::awake()
{
    if (m_camera == nullptr)
    {
        m_camera = new Camera();
        std::cout << "Camera Created" << std::endl;
    }
}

void Scene::start() const {
	for (const auto i : m_inSceneOpaque)
	{
		if (i->getActiveState())
		{
			i->start();
		}
	}
	for (const auto i : m_inSceneTransparent)
	{
		if (i->getActiveState())
		{
			i->start();
		}
	}
}

void Scene::update() const {
	for (const auto i : m_inSceneOpaque)
	{
		if (i->getActiveState())
		{
			i->update();
		}
	}
	for (const auto i : m_inSceneTransparent)
	{
		if (i->getActiveState())
		{
			i->update();
		}
	}
}

void Scene::lateUpdate()
{
	for (auto i : m_toBeAdded)
	{
		m_inSceneOpaque.push_back(i);
	}
	m_toBeAdded.clear();

	for (const auto & i : m_toBeDeleted)
	{
		i->destroy();
	}
	m_toBeDeleted.clear();
}

string Scene::getName()
{
	return m_name;
}

void Scene::instantiate(Object* obj)
{
	m_toBeAdded.push_back(obj);
}

vec3 Scene::getAmbientColor() const {
	return m_ambientColor;
}

float Scene::getAmbientPower() const {
	return m_ambientPower;
}

void Scene::setDirectionalLight(DirectionalLight *directionalLight) {
	m_directionalLight = directionalLight;
}

DirectionalLight * Scene::getDirectionalLight() const {
	return m_directionalLight;
}

void Scene::addPointLight(PointLight *pointLight) {
	m_pointLights.push_back(pointLight);
}

vector<PointLight *> Scene::getPointLights() {
	return m_pointLights;
}

Camera* Scene::getCamera() const {
    return m_camera;
}

void Scene::setSkybox(const vector<const char*> &paths)
{
	MeshRenderer* m;
	if (m_skybox == nullptr)
	{
		m_skybox = new Object;
		m = m_skybox->addComponent<MeshRenderer>();
	}
	else
	{
		m = m_skybox->getComponent<MeshRenderer>();
	}
	m->loadCubeMap(paths);
	m->setShaderProgram("assets/defaultAssets/Shaders/skybox.vert", "assets/defaultAssets/Shaders/skybox.frag");
}

void Scene::loadDefaultSkybox()
{
	const vector<const char*> paths = {
	"assets/defaultAssets/Skybox/right.jpg",
	"assets/defaultAssets/Skybox/left.jpg",
	"assets/defaultAssets/Skybox/top.jpg",
	"assets/defaultAssets/Skybox/bottom.jpg",
	"assets/defaultAssets/Skybox/front.jpg",
	"assets/defaultAssets/Skybox/back.jpg"
	};

	setSkybox(paths);
}

void Scene::makeTransparent(Object *object) {
	erase(m_inSceneOpaque, object);
	m_inSceneTransparent.push_back(object);
	ranges::sort(m_inSceneTransparent, [this](const Object* a, const Object* b) {
		const float distance_a = glm::length(m_camera->getWorldPosition() - a->getWorldPosition());
		const float distance_b = glm::length(m_camera->getWorldPosition() - b->getWorldPosition());
		return distance_a > distance_b;
	});
}

void Scene::makeOpaque(Object *object) {
	erase(m_inSceneTransparent, object);
	m_inSceneOpaque.push_back(object);
}
