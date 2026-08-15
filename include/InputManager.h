//
// Created by AwokenOwen on 3/25/26.
//

#pragma once
#include "Event.h"
#include "LinearMath.h"
#include "WindowManager.h"

/**
 * @brief Singleton Macro
 */
#define Input InputManager::getInstance()

// Forward declaration of callback functions
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

/**
 * @brief Used to send information about GLFW keyboard input through keyboard events
 */
struct KeyboardContext
{
private:
	/**
	 * @brief A code referring to which key was pressed on the keyboard
	 */
	int m_key;
	/**
	 * @brief A code referring to which key was pressed on the keyboard specific to OS
	 */
	int m_action;
	/**
	 * @brief A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 */
	int m_scancode;
	/**
	 * @brief A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	int m_mods;
public:
	/**
	 * @brief Keyboard Context Constructor
	 *
	 * @param key A code referring to which key was pressed on the keyboard
	 * @param action A code referring to which key was pressed on the keyboard specific to OS
	 * @param scancode A code that is non OS specific on the key pressed
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	KeyboardContext(const int key, const int action, const int scancode, const int mods) {
		this->m_key = key;
		this->m_action = action;
		this->m_scancode = scancode;
		this->m_mods = mods;
	}

	/**
	 * @brief Getter for key
	 *
	 * @return int
	 */
	[[nodiscard]] int getKey() const {
		return m_key;
	}

	/**
	 * @brief Getter for Action
	 *
	 * @return int
	 */
	[[nodiscard]] int getAction() const {
		return m_action;
	}

	/**
	 * @brief Getter for Scancode
	 *
	 * @return int
	 */
	[[nodiscard]] int getScancode() const {
		return m_scancode;
	}

	/**
	 * @brief Getter for mods
	 *
	 * @return int
	 */
	[[nodiscard]] int getMods() const {
		return m_mods;
	}
};

/**
 * @brief Used to send information about GLFW mouse button input through keyboard events
 */
struct MouseButtonContext
{
private:
	/**
	 * @brief A code referring to the mouse button pressed
	 */
	int m_button;
	/**
	 * @brief A code referring to which key was pressed on the keyboard specific to OS
	 */
	int m_action;
	/**
	 * @brief A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	int m_mods;
public:
	/**
	 * @brief Mouse Button Context Constructor
	 *
	 * @param button A code referring to the mouse button pressed
	 * @param action A code referring to which key was pressed on the keyboard specific to OS
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	MouseButtonContext(const int button, const int action, const int mods)
	{
		this->m_button = button;
		this->m_action = action;
		this->m_mods = mods;
	}

	/**
	 * @brief Getter for Button
	 *
	 * @return int
	 */
	[[nodiscard]] int getButton() const
	{
		return m_button;
	}

	/**
	 * @brief Getter for Action
	 *
	 * @return int
	 */
	[[nodiscard]] int getAction() const
	{
		return m_action;
	}

	/**
	 * @brief Getter for Mods
	 *
	 * @return int
	 */
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
	friend class GameManager;
public:
	/**
	 * @brief InputManager Singleton get function
	 *
	 * @return static InputManager Instance
	 */
	static InputManager& getInstance();

	EVENT_ACCESSORS(MouseMoveEvent, Vector2, Vector2)
	EVENT_ACCESSORS(KeyboardEvent, KeyboardContext)
	EVENT_ACCESSORS(MouseButtonEvent, MouseButtonContext)
	EVENT_ACCESSORS(ScrollEvent, Vector2)

private:
	/**
	 * @brief Private Constructor for singleton
	 */
	InputManager() = default;

	/**
	 * @brief Starts up and initializes everything the input manager needs to run.
	 *
	 * @return Returns 0 if successful and 1 if failed
	 */
	int initialize();

	/**
	 * @brief shuts down the input manager at the end of the game loop
	 */
	void terminate();

	/**
	 * @brief Takes in position of the mouse and calls the mouseMoveEvent with a vec2 for mouse position and vec2 for the mouse delta (the change in mouse position between frames)
	 *
	 * @param posX X position of the mouse
	 * @param posY Y position of the mouse
	 */
	void mouseMoveInput(float posX, float posY);

	/**
	 * @brief Takes in the key, scancode, action, and mods and turns it into a KeyboardContext and calls the keyboardEvent sending the KeyboardContext
	 *
	 * @param key A code referring to which key was pressed on the keyboard
	 * @param scancode A code referring to which key was pressed on the keyboard specific to OS
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void keyboardInputs(int key, int scancode, int action, int mods);

	/**
	 * @brief Takes in the button, action, and mods and turns it into a MouseContext and calls the mouseButtonEvent sending the MouseContext
	 *
	 * @param button A code referring to the mouse button pressed
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void mouseButtonInputs(int button, int action, int mods);

	/**
	 * @brief Takes in the offset of the scroll through two floats, converts it to a vec2 and sends it in the scrollEvent
	 *
	 * @param xOffset The offset of the scroll on the X Axis
	 * @param yOffset The offset of the scroll on the Y Axis
	 */
	void scrollInput(double xOffset, double yOffset);

	// Friend declaration of callback functions to allow private input manager calls
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

	/**
	 * @brief Event called when based on mouse position and delta
	 */
	Event<Vector2, Vector2> MouseMoveEvent{};
	/**
	 * @brief Event gives KeyboardContext based on key
	 */
	Event<KeyboardContext> KeyboardEvent{};
	/**
	 * @brief Event gives MouseButtonContext on the mouse button
	 */
	Event<MouseButtonContext> MouseButtonEvent{};
	/**
	 * @brief Event give a vec2 of scroll delta
	 */
	Event<Vector2> ScrollEvent{};

	/**
	 * @brief mouse variable for if it's the first time the mouse moved
	 */
	bool m_firstMouse{true};

	/**
	 * @brief last position of mouse
	 */
	Vector2 m_lastMousePos;
};