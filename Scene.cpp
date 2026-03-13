#include "Scene.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Camera.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "MeshRenderer.h"
#include "Material.h"

Scene::Scene()
{
	ambientColor = vec3(1.0f);
	ambientPower = 0.1f;
	directionalLight = nullptr;
}

void Scene::Awake()
{
    if (camera == nullptr)
    {
        camera = new Camera();
        std::cout << "Camera Created" << std::endl;
    }
	for (int i = 0; i < inScene.size(); i++)
	{
		if (inScene[i]->getActiveState())
		{
			inScene[i]->Awake();
		}
	}
}

void Scene::Start()
{
	for (int i = 0; i < inScene.size(); i++)
	{
		if (inScene[i]->getActiveState())
		{
			inScene[i]->Start();
		}
	}
}

void Scene::Update()
{
	DrawSkybox();
	for (int i = 0; i < inScene.size(); i++)
	{
		if (inScene[i]->getActiveState())
		{
			inScene[i]->Update();
		}
	}
}

void Scene::LateUpdate()
{
	for (int i = 0; i < inScene.size(); i++)
	{
		if (inScene[i]->getActiveState())
		{
			inScene[i]->LateUpdate();
		}
	}

	for (int i = 0; i < toBeAdded.size(); i++)
	{
		inScene.push_back(toBeAdded[i]);
	}
	toBeAdded.clear();

	for (int i = 0; i < toBeDeleted.size(); i++)
	{
		toBeDeleted[i]->Destroy();
	}
	toBeDeleted.clear();
}

string Scene::getName()
{
	return name;
}

void Scene::Instantiate(Object* obj)
{
	toBeAdded.push_back(obj);
}

Camera* Scene::getCamera()
{
    return camera;
}

void Scene::setSkybox(vector<const char*> paths)
{
	MeshRenderer* m;
	if (skybox == nullptr)
	{
		skybox = new Object;
		m = skybox->addComponent<MeshRenderer>();
	}
	else
	{
		m = skybox->getComponent<MeshRenderer>();
	}
	m->loadCubeMap(paths);
	m->material->setShaderProgram("assets/defaultAssets/skybox.vert", "assets/defaultAssets/skybox.frag");
	skybox->setActive(false);
}

void Scene::loadDefaultSkybox()
{
	vector<const char*> paths = {
	"assets/defaultAssets/Skybox/right.jpg",
	"assets/defaultAssets/Skybox/left.jpg",
	"assets/defaultAssets/Skybox/top.jpg",
	"assets/defaultAssets/Skybox/bottom.jpg",
	"assets/defaultAssets/Skybox/front.jpg",
	"assets/defaultAssets/Skybox/back.jpg"
	};

	setSkybox(paths);
}

void Scene::DrawSkybox()
{
	if (skybox != nullptr)
	{
		skybox->Update();
	}
}
