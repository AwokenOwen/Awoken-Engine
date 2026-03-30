#include "WindowManager.h"
#include "InputManager.h"
#include <ext/matrix_clip_space.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int m_windowWidth;
int m_windowHeight;
int m_viewportWidth;
int m_viewportHeight;
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
int WindowManager::getWindowWidth()
{
	return m_windowWidth;
}
int WindowManager::getWindowHeight()
{
	return m_windowHeight;
}

//Clear the color and depth buffers
void WindowManager::clear()
{
	//rendering commands 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::swap() const {
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
	return glm::perspective(glm::radians(80.0f), static_cast<float>(Window.getWindowWidth()) / static_cast<float>(Window.getWindowHeight()), 0.1f, 100.0f);;
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

	m_windowWidth = mode->width;
	m_windowHeight = mode->height;

	p_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Game Engine", nullptr, nullptr);

	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
	if (fixedAspect)
	{
		if (static_cast<float>(width) / static_cast<float>(height) > aspectRatio)
		{
			m_viewportHeight = height;
			m_viewportWidth = static_cast<int>(m_windowHeight * aspectRatio);
		}
		else if (static_cast<float>(width) / static_cast<float>(height) < aspectRatio)
		{
			m_viewportWidth = width;
			m_viewportHeight = static_cast<int>(m_windowWidth * (1.0f/aspectRatio));
		}
		else
		{
			m_viewportWidth = width;
			m_viewportHeight = height;
		}
	}
	else
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
	}
	glViewport(static_cast<int>((width - m_viewportWidth) / 2.0f), static_cast<int>((height - m_viewportHeight) / 2.0f), m_viewportWidth, m_viewportHeight);
}
