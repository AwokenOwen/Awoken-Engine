//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include <string>

#include "Manager.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

/**
 * @brief Singleton Macro
 */
#define Window WindowManager::getInstance()
class WindowManager : public Manager{
    friend class GameManager;
public:
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the WindowManager
     */
    static WindowManager& getInstance();
    /**
     * G@brief Getter for the GLFW window
     *
     * @return GLFW window*
     */
    [[nodiscard]] GLFWwindow* getWindow() const;
    /**
     * @brief Setting the string of the title of the Window Bar on window creation. Needs to be called before init
     *
     * @param title
     */
    void setWindowTitle(const std::string &title);

    [[nodiscard]] float getAspectRatio() const;
private:
    /**
     * @brief Starts the Window Manager and creates the window for the game
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    int initialize() override;
    /**
     * @brief Shuts down the Window Manager freeing and necessary data
     */
    void terminate() override;
    /**
     * @brief Private default constructor for singleton functionality
     */
    WindowManager() = default;
    /**
     * @brief Private default deconstructor for singleton functionality
     */
    ~WindowManager() override = default;

    /**
     * @brief Clear the color and depth buffers
     */
    static void clear();
    /**
     * @brief Swap buffers and poll events
     */
    void swap() const;
    /**
     * @brief Helper function to create the window for the game
     *
     * @return 1 if successful and 0 if unsuccessful
     */
    int createWindow();
    /**
     * @brief Callback called when the window size changes used to change the OpenGL viewport
     *
     * @param width New width of the window
     * @param height New height of the window
     */
    void framebuffer_size_callback(int width, int height);
    /**
     * @brief Static callback function for GLFW to call the correct function that will update the window
     *
     * @param window The window of the callback is on
     * @param width New width of the window
     * @param height New height of the window
     */
    static void framebuffer_size_callback_static(GLFWwindow* window, int width, int height);

    /**
     * @brief Pointer that holds the GLFW window
     */
    GLFWwindow* p_window{};

    /**
     * @brief Window and viewport variables for defining the window size and OpenGL viewport
     */
    int m_windowWidth{};
    int m_windowHeight{};
    int m_viewportWidth{};
    int m_viewportHeight{};

    /**
     * @brief Bool that defines whether the aspect ratio must stay the same or not
     */
    bool m_fixedAspectRatio{};
    /**
     * @brief The aspect ratio the viewport must stay at if the aspect ratio is fixed
     */
    float m_aspectRatio{16.0f/9.0f};

    /**
     * @brief The title of the window on window bar
     */
    std::string m_windowTitle{"Game Engine"};
};
