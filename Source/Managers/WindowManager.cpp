//
// Created by AwokenOwen on 4/13/26.
//

#include "WindowManager.h"

#include "LogManager.h"
#include "ResourceManager.h"

int WindowManager::initialize() {
    // Init GLFW
    glfwInit();
    // Make the window and check if successful
    if(createWindow()) {
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // Log Done
    Log.log("WindowManager Initialized");
    return 0;
}

void WindowManager::terminate() {
    // Log Done
    Log.log("WindowManager Terminated");
}

WindowManager & WindowManager::getInstance() {
    // Get singleton instance and return it
    static WindowManager singleton;
    return singleton;
}

// Getter for the window pointer
GLFWwindow * WindowManager::getWindow() const {
    return p_window;

}

// Setter for the window title
void WindowManager::setWindowTitle(const std::string &title) {
    m_windowTitle = title;
}

float WindowManager::getAspectRatio() const {
    if (m_fixedAspectRatio) {
        return m_aspectRatio;
    }
    return static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
}

int WindowManager::getViewportWidth() const
{
    return m_viewportWidth;
}

int WindowManager::getViewportHeight() const
{
    return m_viewportHeight;
}

void WindowManager::resetViewport()
{
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
}

void WindowManager::clear() {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::swap() const {
    // Swap buffers and poll events
    glfwSwapBuffers(p_window);
    glfwPollEvents();
}

int WindowManager::createWindow() {
    // Getting primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // Getting the video mode of the monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Setting Hints
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // Loading variables for screen loading
    m_windowWidth = mode->width;
    m_windowHeight = mode->height;
    m_viewportWidth = m_windowWidth;
    m_viewportHeight = m_windowHeight;

    // Creating the window
    p_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), nullptr, nullptr);

    // Null check
    if (p_window == nullptr)
    {
        Log.logError("Failed to create GLFW window.");
        glfwTerminate();
        return 1;
    }
    // Set context
    glfwMakeContextCurrent(p_window);

    // Disable VSync to uncap framerate
    glfwSwapInterval(0);

    // Glad loading check
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        Log.logError("Failed to initialize GLAD.");
        return 1;
    }

    // Set viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Set GLFW owner for call backs
    glfwSetWindowUserPointer(p_window, this);
    glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback_static);

    return 0;
}

void WindowManager::framebuffer_size_callback(const int width, const int height) {
    // Set window width and height for use later
    m_windowWidth = width;
    m_windowHeight = height;
    if (m_fixedAspectRatio)
    {
        // Making the width and height of the viewport match the fixed aspect ratio
        // If the ratio of the window is GREATER than the fixed ratio then the width
        // is too large and needs to shrink relative to the fixed ratio
        // If the ratio of the window is LESS than the fixed ratio then the heights
        // is too large and needs to shrink relative to the fixed ratio
        if (static_cast<float>(width) / static_cast<float>(height) > m_aspectRatio)
        {
            m_viewportHeight = height;
            m_viewportWidth = static_cast<int>(static_cast<float>(m_windowHeight) * m_aspectRatio);
        }
        else if (static_cast<float>(width) / static_cast<float>(height) < m_aspectRatio)
        {
            m_viewportWidth = width;
            m_viewportHeight = static_cast<int>(static_cast<float>(m_windowWidth) / m_aspectRatio);
        }
        else
        {
            // Just in case the ratio is exact so all cases are met
            m_viewportWidth = width;
            m_viewportHeight = height;
        }
    }
    else
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
    }

    // Set the viewport so that it's in the middle of the window rather than starting at the bottom right
    glViewport((width - m_viewportWidth) / 2, (height - m_viewportHeight) / 2, m_viewportWidth, m_viewportHeight);
    Resource.resizeFrameBuffer("post", m_viewportWidth, m_viewportHeight);
}

// Helper static function that GLFW calls
void WindowManager::framebuffer_size_callback_static(GLFWwindow *window, int width, int height) {
    if (auto* handler = static_cast<WindowManager*>(glfwGetWindowUserPointer(window))) {
        handler->framebuffer_size_callback(width, height);
    }
}
