//
// Created by awokenowen on 4/13/26.
//

#pragma once
#include <string>

#include "AL/alc.h"

/**
 * @brief Singleton Macro
 */
#define Game GameManager::getInstance()

/**
 * @brief Singleton class that runs the engine
 */
class GameManager {
public:
    /**
     * @brief Starts all other managers preparing to start the game
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    int initialize();
    /**
     * @brief Shuts down all other managers and frees necessary data
     */
    void terminate();
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the GameManager
     */
    static GameManager& getInstance();

    /**
     * @brief Runs the game loop
     */
    void run();
    /**
     * @brief Getter for delta time
     *
     * @return The delta time defined by the game loop
     */
    [[nodiscard]] float getDeltaTime() const;
    /**
     * @brief Sets the maximum the framerate the game loop will run at. If 0 then it's set to unlimited
     *
     * @param framerate The new maximum framerate the game will run at
     */
    void setMaxFramerate(float framerate);
private:
    /**
     * @brief Private default constructor for singleton functionality
     */
    GameManager() = default;
    /**
     * @brief Private default deconstructor for singleton functionality
     */
    ~GameManager() = default;

    /**
     * @brief The time it takes for each frame to run
     */
    float m_deltaTime{0.00001f};
    /**
     * @brief Maximum FPS the game loop will run at
     */
    float m_framerateFactor{};

    /**
     * @brief The OpenAL device object used to play sounds
     */
    ALCdevice* m_device{};
    /**
     * @brief The OpenAL context object used to play sounds
     */
    ALCcontext* m_context{};
};
