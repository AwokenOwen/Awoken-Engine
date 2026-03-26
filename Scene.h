/*
 * Written by: AwokenOwen
 * Last Updated: March 26th 2026
 */

#pragma once
#include <string>
#include <vector>
#include "glm.hpp"

using namespace glm;
using namespace std;

/**
 * @brief Forward Declaration for Camera class
 */
class Camera;
/**
 * @brief Forward Declaration for Object class
 */
class Object;
/**
 * @brief Forward Declaration for DirectionalLight class
 */
class DirectionalLight;
/**
 * @brief Forward Declaration for PointLight class
 */
class PointLight;
/**
 * @brief Class that handles all the objects that need to be currently loaded
 */
class Scene
{
public:
	/**
	 * @brief Default constructor
	 */
	Scene();

	/**
	 * @brief Called once at the start of a scene
	 */
	void awake();
	/**
	 * @brief Called on the first frame if enabled
	 */
	void start() const;
	/**
	 * @brief Called every frame
	 */
	void update() const;
	/**
	 * @brief Called every frame after update
	 */
	void lateUpdate();
	/**
	 * @brief Getter for the name of the Scene
	 *
	 * @return string
	 */
	string getName();

	/**
	 * @brief Used to add an object into the list of objects in the scene
	 *
	 * @param obj The object to be added
	 */
	void instantiate(Object* obj);

	/**
	 * @brief Getter for ambient light color
	 *
	 * @return vec3
	 */
	vec3 getAmbientColor() const;
	/**
	 * @brief Getter for ambient light power
	 *
	 * @return float
	 */
	float getAmbientPower() const;

	/**
	 * @brief Setter for the single Directional light in the scene
	 *
	 * @param directionalLight The new directional light
	 */
	void setDirectionalLight(DirectionalLight* directionalLight);
	/**
	 * @brief Getter for the Directional Light
	 *
	 * @return DirectionalLight
	 */
	DirectionalLight* getDirectionalLight() const;

	/**
	 * @brief Function to add a point light into the scene
	 *
	 * @param pointLight The Point light to be added
	 */
	void addPointLight(PointLight* pointLight);
	/**
	 * @brief Getter for the point light vector
	 *
	 * @return vector<PointLight*>
	 */
	vector<PointLight*> getPointLights();

	/**
	 * @brief Getter for the active camera
	 *
	 * @return Camera*
	 */
	Camera* getCamera() const;

	/**
	 * @brief Setter for the active skybox
	 *
	 * @param paths Vector of paths to each image file of the cube map skybox
	 */
	void setSkybox(const vector<const char*> &paths);
	/**
	 * @brief Grabs the default skybox images and makes a skybox of it
	 */
	void loadDefaultSkybox();

private:
	/**
	 * @brief Vector of objects in the scene
	 */
	vector<Object*> m_inScene;
	/**
	 * @brief Vector of objects that need to be added
	 */
	vector<Object*> m_toBeAdded;
	/**
	 * @brief Vector of objects that need to be deleted
	 */
	vector<Object*> m_toBeDeleted;

	/**
	 * @brief The name of the scene
	 */
	string m_name;

	/**
	 * @brief The ambient color of the scene
	 *
	 * || NEED TO CHANGE ON IMPLEMENTING IBL ||
	 */
	vec3 m_ambientColor{};
	/**
	 * @brief The ambient power of the scene
	 *
	 * || NEED TO CHANGE ON IMPLEMENTING IBL ||
	 */
	float m_ambientPower;

	/**
	 * @brief The current directional light in the scene
	 *
	 * || Eventually add ability for multiple ||
	 */
	DirectionalLight* m_directionalLight = nullptr;
	/**
	 * @brief The current point lights in the scene
	 */
	vector<PointLight*> m_pointLights;

	/**
	 * @brief The active camera
	 */
	Camera* m_camera = nullptr;
	/**
	 * @brief The active skybox
	 */
	Object* m_skybox = nullptr;
};