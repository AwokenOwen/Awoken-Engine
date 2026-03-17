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
	int initialize();

	//free any data on the heap
	static void terminate();

	//Singleton get function
	static WindowManager& getInstance();

	//get function for the private window variable
	GLFWwindow* getWindow();

	//get functions for the width and height of the screen
	static int getWidth();
	static int getHeight();
	[[nodiscard]] float getPixelRatio() const;

	//Clear the color and depth buffers
	static void clear();

	//Swap buffers and poll events
	void Swap() const;

	void setMaximized(bool maximized = true) const;

	static mat4 getPerspectiveMatrix();
	mat4 getOrthographicMatrix();

	static void setFixedAspect(bool fixed = true);
	static void setAspectRatio(float aspect);

private:
	//Private constructor for singleton functionality
	WindowManager();

	//creates the window by grabbing the primary monitor setting the window to be 
	//"windowed fullscreen", then initializing glad, then finally creating the window
	int createWindow();
	GLFWwindow* p_window{};

	float m_monitorHeight{};
};