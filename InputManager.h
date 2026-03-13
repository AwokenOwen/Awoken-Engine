/*
 * Written by: AwokenOwen
 * Last Updated: March 13th 2026
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

class InputManager
{
public:
	// start the input manager
	int initialize(GLFWwindow* window);

	// free any data on the heap
	int terminate();

	// singleton get function
	static InputManager& getInstance();

	// Input function for mouse movement
	void mouseMoveInput(float posX, float posY);
	// Input function for keyboard inputs
	void keyboardInputs(int key, int scancode, int action, int mods);
	// Input function for mouse buttons
	void mouseButtonInputs(int button, int action, int mods);
	// Input function for the scroll
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