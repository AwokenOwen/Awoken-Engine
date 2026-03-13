#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "iostream"

using namespace glm;
using namespace std;

#define Window WindowManager::getInstance()

class WindowManager
{
public:
	//Initialize the Window manager and call the create window function
	int Initialize();

	//free any data on the heap
	void Terminate();

	//Singleton get function
	static WindowManager& getInstance();

	//get function for the private window variable
	GLFWwindow* getWindow();

	//get functions for the width and height of the screen
	int getWidth();
	int getHeight();
	float getPixelRatio();

	//Clear the color and depth buffers
	void Clear();

	//Swap buffers and pollevents
	void Swap();

	void setMaximized(bool maximized = true);

	mat4 getPerspectiveMatrix();
	mat4 getOrthographicMatrix();

	void setFixedAspect(bool fixed = true);

private:
	//Private contructor for singleton functionallity
	WindowManager();

	//creates the window by grabbing the primary monitor setting the window to be 
	//"windowed fullscreen", then initializing glad, then finally creating the window
	int createWindow();
	GLFWwindow* window;

	float monitorHeight;
};