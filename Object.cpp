#include "Object.h"
#include "Component.h"
#include "iostream"
#include <gtx/matrix_decompose.hpp>
#include "WorldManager.h"

//Default contructor adds itself to the world list
Object::Object()
{
	//set default variables and instantiate
	enabled = true;

	localPosition = vec3(0, 0, 0);
	localRotation = quat(vec3(0, 0, 0));
	localScale = vec3(1, 1, 1);

	worldPosition = vec3(0, 0, 0);
	worldRotation = quat(vec3(0, 0, 0));
	worldScale = vec3(1, 1, 1);

	World.Instantiate(this);
}

//called once at the start of the frame it is enabled on
void Object::OnEnable()
{
	for (int i = 0; i < componentsSize; i++)
	{
		if (components[i]->getActiveState())
		{
			components[i]->onEnable();
		}
	}

	enabled = true;
}

//called on the first frame if enabled
void Object::Start()
{
	for (int i = 0; i < componentsSize; i++)
	{
		if (components[i]->getActiveState())
		{
			components[i]->start();
		}
	}
}

//called every frame
void Object::Update()
{
	worldModelMatrix();
	if (firstFrame)
	{
		OnEnable();
		firstFrame = false;
	}
	for (int i = 0; i < componentsSize; i++)
	{
		if (components[i]->getActiveState())
		{
			components[i]->update();
		}
	}
}

//called on the last frame it is enabled
void Object::OnDisable()
{
	for (int i = 0; i < componentsSize; i++)
	{
		if (components[i]->getActiveState())
		{
			components[i]->onDisable();
		}
	}
	enabled = false;
}

void Object::Destroy()
{
	OnDisable();

	components.clear();

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Destroy();
	}
	delete this;
}

//return parent object if there is one
Object* Object::getParent()
{
	return parent;
}

void Object::setActive(bool activeState)
{
	if (activeState != enabled)
	{
		if (activeState)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
		enabled = activeState;
	}
}

bool Object::getActiveState()
{
	return enabled;
}

mat4 Object::localModelMatrix()
{
	mat4 model = mat4(1.0f);

	model = translate(model, GetLocalPosition());
	model = rotate(model, glm::angle(GetLocalRotation()), glm::axis(GetLocalRotation()));
	model = scale(model, GetLocalScale());

	return model;
}

mat4 Object::worldModelMatrix()
{
	mat4 worldModelMatrix = localModelMatrix();

	Object* current = parent;
	while (current != nullptr) {
		worldModelMatrix = current->localModelMatrix() * worldModelMatrix;
		current = current->parent;
	}
	
	forward = vec3(worldModelMatrix[0][2], worldModelMatrix[1][2], worldModelMatrix[2][2]);
	up = vec3(worldModelMatrix[0][1], worldModelMatrix[1][1], worldModelMatrix[2][1]);
	right = vec3(worldModelMatrix[0][0], worldModelMatrix[1][0], worldModelMatrix[2][0]);

	decompose(worldModelMatrix, worldScale, worldRotation, worldPosition, skew, perspective);

	return worldModelMatrix;
}

void Object::setParent(Object* parent)
{
	this->parent = parent;
}


int Object::addChild(Object* child) 
{
	Object* current = parent;
	while (current != nullptr)
	{
		if (current == child)
		{
			std::cout << "Error: Child is currently a parent of Object\n";
			return 1;
		}
		current = current->getParent();
	}

	child->setParent(this); 
	children.push_back(child);
	return 0;
}

vec3 Object::GetWorldPosition()
{
	worldModelMatrix();
	return worldPosition;
}

quat Object::GetWorldRotation()
{
	worldModelMatrix();
	return worldRotation;
}

vec3 Object::GetWorldScale()
{
	worldModelMatrix();
	return worldScale;
}

vec3 Object::GetForward()
{
	return forward;
}

vec3 Object::GetRight()
{
	return right;
}

vec3 Object::GetUp()
{
	return up;
}

vec3 Object::GetLocalPosition()
{
	return localPosition;
}

quat Object::GetLocalRotation()
{
	return localRotation;
}

vec3 Object::GetLocalScale()
{
	return localScale;
}

void Object::SetLocalPosition(vec3 location)
{
	localPosition = location;
}

void Object::SetLocalRotation(quat rotation)
{
	localRotation = rotation;
}

void Object::SetLocalScale(vec3 scale)
{
	localScale = scale;
}

void Object::Translate(vec3 translation)
{
	SetLocalPosition(GetLocalPosition() + translation);
}

void Object::Rotate(vec3 eularRotation)
{
	SetLocalRotation(GetLocalRotation() + quat(eularRotation));
}

void Object::Scale(vec3 scaleFactors)
{
	SetLocalScale(GetLocalScale() * scaleFactors);
}
