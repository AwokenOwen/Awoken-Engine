//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include <string>

#include "Event.h"
#include "nlohmann/json.hpp"

/**
 * @brief Singleton Macro
 */
#define World WorldManager::getInstance()

class CameraComponent;
class LightComponent;
struct Scene;
class Renderer;
class Object;

/**
 * @brief Singleton class that handles all Frame-buffers and the World events
 */
class WorldManager {
    friend class GameManager;
    friend class ResourceManager;
public:
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the GameManager
     */
    static WorldManager& getInstance();

    /**
     * @brief sets the name of the base scene needed to start the engine
     *
     * @param name Name of the scene in the resource manager scene map
     */
    void setBaseScene(const std::string& name);

    void setMainCamera(CameraComponent* camera);
    [[nodiscard]] CameraComponent* getMainCamera() const;

private:
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
     * @brief A setup for the scene called when a scene is loaded to make sure everything can run properly
     */
    void awake();
    /**
     * @brief Caller function for the update event sent to all objects
     */
    void update() const;
    /**
    * @brief Private default constructor for singleton functionality
    */
    WorldManager() = default;
    /**
    * @brief Private default deconstructor for singleton functionality
    */
    ~WorldManager() = default;

    void drawPostprocess() const;

    std::string m_baseScene{"default"};

    std::vector<LightComponent*> m_lights{};
    CameraComponent* m_mainCamera{};
};
