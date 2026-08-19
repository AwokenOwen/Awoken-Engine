//
// Created by AwokenOwen on 4/13/26.
//

#include "GameManager.h"

#include <chrono>

#include "InputManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "WorldManager.h"

int GameManager::initialize() {
    // Other Manager Initializations
    if (Log.initialize() == 1 || Window.initialize() == 1 || World.initialize() == 1 || Input.initialize() == 1 ||  Resource.initialize()  == 1/* || other manager inits == 1 */) {
        exit(-1);
    }


    // Grab the default audio device
    m_device = alcOpenDevice(nullptr);
    // Error check if no device was grabbed
    if (!m_device)
    {
        // Log the error
        Log.logError("Could not find a default audio device.");
        exit(-1);
    }
    // Create a context from the device
    m_context = alcCreateContext(m_device, nullptr);
    // Error check if the context failed
    if (!m_context)
    {
        // Log the error
        Log.logError("Could not create a audio context.");
        exit(-1);
    }
    // Make the created context the current context
    alcMakeContextCurrent(m_context);
    // Error check if context doesn't works
    if (alcGetError(m_device) != ALC_NO_ERROR)
    {
        // Log the error... We have a bigger problem
        Log.logError("Could not make context the current context.");
        exit(-1);
    }

    // Game Manager Done
    Log.log("GameManager Initialized.");

    // Log all managers done
    Log.log("All Managers initialized.");

    // Return success
    return 0;
}

void GameManager::terminate() {
    // Terminating all Managers
    Window.terminate();
    World.terminate();
    Input.terminate();
    Resource.terminate();
    // other manager terminate

    // Destroy OpenAL data (doesn't really matter because the engine is terminating)
    alcCloseDevice(m_device);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_context);

    // Log all managers done
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

    // Set the window to maximized mode
    Window.setMaximized();

    // Run awake on the world manager to load the starting scene
    World.awake();
    // Game loop defining delta time and updating the game
    auto lastTime = static_cast<float>(glfwGetTime());
    // While window open
    while (!glfwWindowShouldClose(Window.getWindow())) {
        // Clear window
        Window.clear();

        // Run the the update loop
        World.update();

        // Swap buffers
        Window.swap();
        // Set delta time
        m_deltaTime = static_cast<float>(glfwGetTime()) - lastTime;
        // If max framerate is 0, the framerate is infinite so skip sleep
        if (m_framerateFactor != 0.0)
            // Sleep to keep the max framerate
            std::this_thread::sleep_for(std::chrono::duration<float> (std::max(0.0f, m_framerateFactor - m_deltaTime)));
        lastTime = static_cast<float>(glfwGetTime());
    }

    // Log the update loop has ended
    Log.log("Finished Game Loop");
}

// Getter for delta time
float GameManager::getDeltaTime() const {
    // incase m_deltaTimes ends up being 0 and will cause errors
    return std::max(m_deltaTime, 0.00001f);
}

// Setter for the max framerate
void GameManager::setMaxFramerate(const float framerate) {
    if (framerate <= 0.0f) {
        // setting this to 0 will make max FPS unlimited
        m_framerateFactor = 0.0f;
        return;
    }
    // The min time in seconds a frame will take
    m_framerateFactor = 1.0f / framerate;
}