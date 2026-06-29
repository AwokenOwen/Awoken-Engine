//
// Created by awokenowen on 4/13/26.
//

#pragma once
#include <string>

#include "Manager.h"
#include "AL/alc.h"

/**
 * @brief Singleton Macro
 */
#define Game GameManager::getInstance()

/**
 * @brief Singleton class that runs the engine
 */
class GameManager : public Manager {
public:
    /**
     * @brief Starts all other managers preparing to start the game
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    // ReSharper disable once CppOverrideWithDifferentVisibility
    int initialize() override;
    /**
     * @brief Shuts down all other managers and frees necessary data
     */
    // ReSharper disable once CppOverrideWithDifferentVisibility
    void terminate() override;
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
    [[nodiscard]] double getDeltaTime() const;
    /**
     * @brief Sets the maximum the framerate the game loop will run at. If 0 then it's set to unlimited
     *
     * @param framerate The new maximum framerate the game will run at
     */
    void setMaxFramerate(double framerate);
private:
    /**
     * @brief Private default constructor for singleton functionality
     */
    GameManager() = default;
    /**
     * @brief Private default deconstructor for singleton functionality
     */
    ~GameManager() override = default;

    /**
     * @brief The time it takes for each frame to run
     */
    double m_deltaTime{};
    /**
     * @brief Maximum FPS the game loop will run at
     */
    double m_framerateFactor{};

    ALCdevice* m_device{};
    ALCcontext* m_context{};
};
