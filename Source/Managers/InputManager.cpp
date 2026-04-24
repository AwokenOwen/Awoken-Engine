#include "InputManager.h"
#include "LogManager.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

//start the input manager
int InputManager::initialize()
{
	glfwSetKeyCallback(Window.getWindow(), key_callback);
	glfwSetCursorPosCallback(Window.getWindow(), mouse_cursor_callback);
	glfwSetMouseButtonCallback(Window.getWindow(), mouse_button_callback);
	glfwSetScrollCallback(Window.getWindow(), scroll_callback);

	Log.log("InputManager initialized");

	return 0;
}

//free any data on the heap
void InputManager::terminate()
{
	Log.log("InputManager terminated");
}

//singleton get function
InputManager& InputManager::getInstance()
{
	static InputManager single;
	return single;
}

// Input function for mouse movement
void InputManager::mouseMoveInput(float posX, float posY)
{
	if (m_firstMouse)
	{
		m_lastX = posX;
		m_lastY = posY;
		m_firstMouse = false;
	}

	const float offsetX = posX - m_lastX;
	const float offsetY = posY - m_lastY;

	m_lastX = posX;
	m_lastY = posY;

	m_mouseMoveEvent.callEvent(Vector2(posX, posY), Vector2(offsetX, offsetY), this);
}

// Input function for keyboard inputs
void InputManager::keyboardInputs(int key, int scancode, int action, int mods)
{
	m_keyboardEvent.callEvent(KeyboardContext(key, action, mods), this);
}

// Input function for mouse button inputs
void InputManager::mouseButtonInputs(int button, int action, int mods)
{
	m_mouseButtonEvent.callEvent(MouseButtonContext(button, action, mods), this);
}

void InputManager::scrollInput(double xOffset, double yOffset)
{
	m_scrollEvent.callEvent(Vector2(static_cast<float>(xOffset), static_cast<float>(yOffset)), this);
}

#pragma region callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input.keyboardInputs(key, scancode, action, mods);
}

void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
	Input.mouseMoveInput(static_cast<float>(xPos), static_cast<float>(yPos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input.mouseButtonInputs(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	Input.scrollInput(xOffset, yOffset);
}
#pragma endregion
