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
            std::this_thread::sleep_for(std::chrono::duration<double> (std::max(0.0, m_framerateFactor - m_deltaTime)));
        lastTime = glfwGetTime();
    }

    // Log Done
    Log.log("Finished Game Loop");
}

// Getter for delta time
double GameManager::getDeltaTime() const {
    return m_deltaTime;
}

// Setter for the max framerate
void GameManager::setMaxFramerate(const double framerate) {
    if (framerate <= 0.0) {
        // setting this to 0 will make max FPS unlimited
        m_framerateFactor = 0.0;
        return;
    }
    // The min time in seconds a frame will take
    m_framerateFactor = 1.0 / framerate;
}