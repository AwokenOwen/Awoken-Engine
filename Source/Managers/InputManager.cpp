#include "InputManager.h"
#include "LogManager.h"


//start the input manager
int InputManager::initialize()
{
	// Set the call back functions
	glfwSetKeyCallback(Window.getWindow(), key_callback);
	glfwSetCursorPosCallback(Window.getWindow(), mouse_cursor_callback);
	glfwSetMouseButtonCallback(Window.getWindow(), mouse_button_callback);
	glfwSetScrollCallback(Window.getWindow(), scroll_callback);

	// Log init done
	Log.log("InputManager initialized");

	return 0;
}

void InputManager::terminate()
{
	// Nothing to be done so log done
	Log.log("InputManager terminated");
}

InputManager& InputManager::getInstance()
{
	// Make the singleton and return it
	static InputManager single;
	return single;
}

void InputManager::mouseMoveInput(const float posX, const float posY)
{
	// Check if this is the first mouse movement to prevent huge one frame jumps
	if (m_firstMouse)
	{
		// Set the last position to the current position
		m_lastMousePos[0] = posX;
		m_lastMousePos[1] = posY;
		// Tell the manager it's no longer the first movement
		m_firstMouse = false;

		// Call the event with 0 offset
		MouseMoveEvent.call(Vector2(posX, posY), Vector2(0, 0));
		return;
	}

	// Calculate offset
	const float offsetX = posX - m_lastMousePos[0];
	const float offsetY = posY - m_lastMousePos[1];

	// Set the last position to be this one
	m_lastMousePos[0] = posX;
	m_lastMousePos[1] = posY;

	// Call the event with the new position and offset
	MouseMoveEvent.call(Vector2(posX, posY), Vector2(offsetX, offsetY));
}

void InputManager::keyboardInputs(const int key, const int scancode, const int action, const int mods)
{
	// Create Keyboard Context from the information from the callback, ignoring scancode
	KeyboardEvent.call(KeyboardContext(key, action, scancode, mods));
}

// Input function for mouse button inputs
void InputManager::mouseButtonInputs(const int button, const int action, const int mods)
{
	MouseButtonEvent.call(MouseButtonContext(button, action, mods));
}

void InputManager::scrollInput(const double xOffset, const double yOffset)
{
	ScrollEvent.call(Vector2(static_cast<float>(xOffset), static_cast<float>(yOffset)));
}

#pragma region callbacks
void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
	// Send callback data from GLFW to the input manager to use
	Input.keyboardInputs(key, scancode, action, mods);
}

void mouse_cursor_callback(GLFWwindow* window, const double xPos, const double yPos)
{
	// Send callback data from GLFW to the input manager to use
	Input.mouseMoveInput(static_cast<float>(xPos), static_cast<float>(yPos));
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods)
{
	// Send callback data from GLFW to the input manager to use
	Input.mouseButtonInputs(button, action, mods);
}

void scroll_callback(GLFWwindow* window, const double xOffset, const double yOffset)
{
	// Send callback data from GLFW to the input manager to use
	Input.scrollInput(xOffset, yOffset);
}
#pragma endregion
