#pragma once
#include <string>
#include <vector>
#include "glm.hpp"

using namespace glm;
using namespace std;

class Camera;
class Object;
class DirectionalLight;
class PointLight;
class Scene
{
public:
	Scene();

	//called once at the start of a scene
	void Awake();
	//called on the first frame if enabled
	void Start();
	//called every frame
	void Update();
	//called every frame after update
	void LateUpdate();
	//returns the name of the Scene
	string getName();

	void Instantiate(Object* obj);

	Camera* getCamera();

	void setSkybox(vector<const char*> paths);
	void loadDefaultSkybox();

	vec3 ambientColor;
	float ambientPower;

	DirectionalLight* directionalLight;

	vector<PointLight*> pointLights;

private:
	void DrawSkybox();

	vector<Object*> inScene;
	vector<Object*> toBeAdded;
	vector<Object*> toBeDeleted;

	string name;

	Camera* camera = nullptr;
	Object* skybox = nullptr;
};