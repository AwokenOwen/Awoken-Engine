#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/euler_angles.hpp>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

class Component;
class Object
{
public:
	virtual ~Object() = default;

	//Default contructor adds itself to the world list
	Object();

	//called once at the start of a scene
	virtual void Awake();
	//called once at the start of the frame it is enabled on
	virtual void OnEnable();
	//called on the first frame if enabled
	virtual void Start();
	//called every frame
	virtual void Update();
	//called every frame after update
	virtual void LateUpdate();
	//called on the last frame it is enabled
	virtual void OnDisable();
	//to be called when Destroying itself
	virtual void Destroy();

	//get the parent object if there is one
	Object* getParent();

	template <typename T> T* addComponent();
	template <typename T> T* getComponent();

	void setActive(bool activeState);
	bool getActiveState();

	int addChild(Object* child);

	vec3 GetWorldPosition();
	quat GetWorldRotation();
	vec3 GetWorldScale();

	vec3 GetForward();
	vec3 GetRight();
	vec3 GetUp();

	mat4 worldModelMatrix();

	vec3 GetLocalPosition();
	quat GetLocalRotation();
	vec3 GetLocalScale();

	mat4 localModelMatrix();

	void SetLocalPosition(vec3 location);
	void SetLocalRotation(quat rotation);
	void SetLocalScale(vec3 scale);

	void Translate(vec3 translation);
	void Rotate(vec3 eularRotation);
	void Scale(vec3 scaleFactors);

protected:
	Object* parent = nullptr;

	vector<Object*> children;

	void setParent(Object* parent);

	vector<Component*> components;
	int componentsSize = 0;

	bool enabled;

	vec3 localPosition;
	//pitch, yaw, roll, rotations
	quat localRotation;
	vec3 localScale;

	vec3 worldPosition;
	//pitch, yaw, roll, rotations
	quat worldRotation;
	vec3 worldScale;

	vec3 forward;
	vec3 right;
	vec3 up;

	vec3 skew;
	vec4 perspective;

	bool firstFrame = true;
};

template<typename T>
inline T* Object::addComponent()
{
	static_assert(std::is_base_of<Component, T>());

	T* a = new T(this);

	components.push_back(a);
	componentsSize++;
	return a;
}

template<typename T>
inline T* Object::getComponent()
{
	static_assert(std::is_base_of<Component, T>());

	for (int i = 0; i < componentsSize; i++)
	{
		if (reinterpret_cast<T*>(components[i]))
		{
			return reinterpret_cast<T*>(components[i]);
		}
	}

	return nullptr;
}
