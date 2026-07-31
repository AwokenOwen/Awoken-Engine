//
// Created by AwokenOwen on 4/13/26.
//

#include "GameManager.h"

#include <chrono>
#include <thread>

#include "InputManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "WorldManager.h"

int GameManager::initialize() {
    // Other Manager Initializations
    if (Log.initialize() == 1 || Window.initialize() == 1 || World.initialize() == 1 || Input.initialize() == 1 || Resource.initialize() == 1/* || other manager inits == 1 */) {
        exit(-1);
    }

    const std::string devices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

    // Init Audio
    m_device = alcOpenDevice(nullptr);
    if (!m_device)
    {
        Log.logError("Could not find a default audio device.");
        return 1;
    }
    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context)
    {
        Log.logError("Could not create a audio context.");
        return 1;
    }
    alcMakeContextCurrent(m_context);
    if (alcGetError(m_device) != ALC_NO_ERROR )
    {
        Log.logError("Could not make context the current context.");
        return 1;
    }

    // Log Done
    Log.log("GameManager Initialized.");
    Log.log("All Managers initialized.");

    return 0;
}

void GameManager::terminate() {
    // Terminating all Managers
    Window.terminate();
    World.terminate();
    Input.terminate();
    Resource.terminate();
    // other manager terminate

    alcCloseDevice(m_device);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_context);

    // Log Done
    Log.log("All Managers terminated. Closing Engine");
    Log.log("GameManager terminated.");
    // Close the log manager last because we want to be able to make logs until the end
    Log.terminate();
}

GameManager & GameManager::getInstance() {
    // Get static singleton and return it
    static GameManager singleton;
    return singleton;
}

void GameManager::run() {
    // Log start
    Log.log("Starting Game Loop");

    World.awake();
    // Game loop defining delta time and updating the game
    double lastTime = glfwGetTime();
    // While window open
    while (!glfwWindowShouldClose(Window.getWindow())) {
        // Clear window
        Window.clear();

        World.update();

        // Swap buffers
        Window.swap();
        // Set delta time
        m_deltaTime = glfwGetTime() - lastTime;
        // If no max framerate just run right through
        if (m_framerateFactor != 0.0)
            // Sleep to keep the max framerate
            std::this_thread::sleep_for(std::chrono::duration<float> (std::max(0.0f, m_framerateFactor - m_deltaTime)));
        lastTime = glfwGetTime();
    }

    // Log Done
    Log.log("Finished Game Loop");
}

// Getter for delta time
float GameManager::getDeltaTime() const {
    return m_deltaTime;
}

// Setter for the max framerate
void GameManager::setMaxFramerate(const float framerate) {
    if (framerate <= 0.0) {
        // setting this to 0 will make max FPS unlimited
        m_framerateFactor = 0.0;
        return;
    }
    // The min time in seconds a frame will take
    m_framerateFactor = 1.0 / framerate;
}