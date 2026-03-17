#include "WindowManager.h"
#include "InputManager.h"
#include <ext/matrix_clip_space.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int width;
int height;
bool fixedAspect;
float aspectRatio;

//Initialize the Window manager and call the create window function
int WindowManager::initialize()
{
	glfwInit();
	createWindow();
	setMaximized();
	fixedAspect = false;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (Input.initialize(p_window))
	{
		cout << "InputManager failed to start\n";
	}
	cout << "InputManager started\n";

	return 0;
}

//free any data on the heap
void WindowManager::terminate()
{
}

//Singleton get function
WindowManager& WindowManager::getInstance()
{
	static WindowManager single;
	return single;
}

//get function for the private window variable
GLFWwindow* WindowManager::getWindow()
{
	if (p_window == nullptr)
		createWindow();
	return p_window;
}

//get functions for the width and height of the screen
int WindowManager::getWidth()
{
	return width;
}
int WindowManager::getHeight()
{
	return height;
}

float WindowManager::getPixelRatio() const
{
	return static_cast<float>(getHeight()) / m_monitorHeight;
}

//Clear the color and depth buffers
void WindowManager::clear()
{
	//rendering commands 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Swap buffers and pollevents
void WindowManager::Swap() const {
	glfwSwapBuffers(p_window);
	glfwPollEvents(); 
}

void WindowManager::setMaximized(const bool maximized) const {
	if (maximized)
	{
		glfwMaximizeWindow(p_window);
	}
	else {
		glfwRestoreWindow(p_window);
	}
}

mat4 WindowManager::getPerspectiveMatrix()
{
	return glm::perspective(glm::radians(80.0f), (float)Window.getWidth() / (float)Window.getHeight(), 0.1f, 100.0f);;
}

mat4 WindowManager::getOrthographicMatrix()
{
	float aspect = static_cast<float>(getWidth()) / static_cast<float>(getHeight());
	return glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);
}

void WindowManager::setFixedAspect(bool fixed)
{
	fixedAspect = fixed;
}

void WindowManager::setAspectRatio(float aspect)
{
	aspectRatio = aspect;
}

//Private constructor for singleton functionality
WindowManager::WindowManager()
= default;

//creates the window by grabbing the primary monitor setting the window to be 
//"windowed fullscreen", then initializing glad, then finally creating the window
int WindowManager::createWindow()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	width = mode->width; 
	height = mode->height; 
	m_monitorHeight = static_cast<float>(mode->height);

	p_window = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);

	if (p_window == nullptr)
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(p_window);

	//NOTE: Init GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//NOTE: Set OpenGL Viewport
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height)
{
	if (fixedAspect)
	{
		if (static_cast<float>(_width) / static_cast<float>(_height) > aspectRatio)
		{
			height = _height;
			width = int(height * aspectRatio);
		}
		else if (static_cast<float>(_width) / static_cast<float>(_height) < aspectRatio)
		{
			width = _width;
			height = static_cast<int>(width * (1.0f/aspectRatio));
		}
		else
		{
			width = _width;
			height = _height;
		}
	}
	else
	{
		width = _width;
		height = _height;
	}
	glViewport(static_cast<int>((_width - width) / 2.0f), static_cast<int>((_height - height) / 2.0f), width, height);
}
