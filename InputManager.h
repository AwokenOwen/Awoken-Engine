/*
 * Written by: AwokenOwen
 * Last Updated: March 14th 2026
 */

#pragma once
#include "Event.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include <string>

/**
 * @brief Singleton Macro
 */
#define Input InputManager::getInstance()


/**
 * @brief Used to send information about GLFW keyboard input through keyboard events
 */
struct KeyboardContext
{
private:
	int m_key;
	int m_action;
	int m_scancode;
	int m_mods;
	std::string m_name;
public:
	KeyboardContext(const int key, const int action, const int mods) {
		this->m_key = key;
		this->m_action = action;
		this->m_mods = mods;

		m_scancode = glfwGetKeyScancode(key);
		m_name = std::string(glfwGetKeyName(key, 0));
	}

	[[nodiscard]] int getKey() const {
		return m_key;
	}

	[[nodiscard]] int getAction() const {
		return m_action;
	}

	[[nodiscard]] int getScancode() const {
		return m_scancode;
	}

	[[nodiscard]] int getMods() const {
		return m_mods;
	}

	std::string getName() {
		return m_name;
	}
};

/**
 * @brief Used to send information about GLFW mouse button input through keyboard events
 */
struct MouseButtonContext
{
private:
	int m_button;
	int m_action;
	int m_mods;
public:
	MouseButtonContext(const int button, const int action, const int mods)
	{
		this->m_button = button;
		this->m_action = action;
		this->m_mods = mods;
	}

	[[nodiscard]] int getButton() const
	{
		return m_button;
	}

	[[nodiscard]] int getAction() const
	{
		return m_action;
	}

	[[nodiscard]] int getMods() const
	{
		return m_mods;
	}
};

/**
 * @brief Singleton class that handles the GLFW inputs into more user-friendly format
 */
class InputManager
{
public:
	/**
	 * @brief Get Input Manager running for use in game
	 *
	 * Starts up and initializes everything the input manager needs to run. Returns 0 if successful and 1 if failed
	 *
	 * @return int
	 */
	int initialize(GLFWwindow* window);

	/**
	 * @brief shuts down the input manager at the end of the game loop
	 */
	void terminate();

	/**
	 * @brief InputManager Singleton get function
	 *
	 * @return static InputManager Instance
	 */
	static InputManager& getInstance();

	/**
	 * @brief Function called by GLFW for when the mouse moves
	 *
	 * Takes in position of the mouse as two floats and calls the mouseMoveEvent with a vec2 for mouse position and vec2 for the mouse delta (the change in mouse position between frames)
	 *
	 * @param posX X position of the mouse
	 * @param posY Y position of the mouse
	 */
	void mouseMoveInput(float posX, float posY);

	/**
	 * @brief Function called by GLFW for all keyboard inputs
	 *
	 * Takes in the key, scancode, action, and mods and turns it into a KeyboardContext and calls the keyboardEvent sending the KeyboardContext
	 *
	 * @param key A code referring to which key was pressed on the keyboard
	 * @param scancode A code referring to which key was pressed on the keyboard specific to OS
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void keyboardInputs(int key, int scancode, int action, int mods);

	/**
	 * @brief Function called by GLFW for all mouse button inputs
	 *
	 * Takes in the button, action, and mods and turns it into a MouseContext and calls the mouseButtonEvent sending the MouseContext
	 *
	 * @param button A code referring to the mouse button pressed
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void mouseButtonInputs(int button, int action, int mods);

	/**
	 * @brief Function called by GLFW for all scroll inputs
	 *
	 * Takes in the offset of the scroll through two floats, converts it to a vec2 and sends it in the scrollEvent
	 *
	 * @param xOffset The offset of the scroll on the X Axis
	 * @param yOffset The offset of the scroll on the Y Axis
	 */
	void scrollInput(double xOffset, double yOffset);

	// Event gives two vec2: Mouse Position, and Mouse Delta
	Event<glm::vec2, glm::vec2> mouseMoveEvent;
	// Event gives KeyboardContext based on key
	Event<KeyboardContext> keyboardEvent;
	// Event gives MouseButtonContext on the mouse button
	Event<MouseButtonContext> mouseButtonEvent;
	// Event give a vec2 of scroll delta
	Event<glm::vec2> scrollEvent;

private:
	// Private constructor
	InputManager();

	//mouse movement variables
	bool firstMouse = true;
	float lastX, lastY;
};