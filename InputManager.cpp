#include "InputManager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

InputManager::InputManager()
{
	lastX = 0.0f;
	lastY = 0.0f;
}

//start the input manager
int InputManager::initialize(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	return 0;
}

//free any data on the heap
void InputManager::terminate()
{

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
	if (firstMouse)
	{
		lastX = posX;
		lastY = posY;
		firstMouse = false;
	}

	const float offsetX = posX - lastX;
	const float offsetY = posY - lastY;

	lastX = posX;
	lastY = posY;

	mouseMoveEvent.callEvent(glm::vec2(posX, posY), glm::vec2(offsetX, offsetY));
}

// Input function for keyboard inputs
void InputManager::keyboardInputs(int key, int scancode, int action, int mods) 
{
	keyboardEvent.callEvent(KeyboardContext(key, action, mods));
}

// Input function for mouse button inputs
void InputManager::mouseButtonInputs(int button, int action, int mods)
{
	mouseButtonEvent.callEvent(MouseButtonContext(button, action, mods));
}

void InputManager::scrollInput(double xOffset, double yOffset)
{
	scrollEvent.callEvent(glm::vec2(float(xOffset), float(yOffset)));
}

#pragma region callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input.keyboardInputs(key, scancode, action, mods);
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input.mouseMoveInput(float(xpos), float(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input.mouseButtonInputs(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input.scrollInput(xoffset, yoffset);
}
#pragma endregion
