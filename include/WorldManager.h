//
// Created by AwokenOwen on 4/13/26.
//

#pragma once
#include <string>

#include "Manager.h"
#include "Event.h"
#include "nlohmann/json.hpp"

/**
 * @brief Singleton Macro
 */
#define World WorldManager::getInstance()

struct Scene;
class Renderer;
class Object;

/**
 * @brief Singleton class that handles all Frame-buffers and the World events
 */
class WorldManager : public Manager{
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
     * @brief Function to add an object into the game during runtime
     *
     * @param parent The parent of the new Object
     * @return Pointer to the newly created Object
     */
    Object* instantiateObject(Object* parent = nullptr);

    /**
     * @brief Function used to prepare an object to be destroyed and the data freed
     *
     * @param object Object to be destroyed
     */
    void destroyObject(Object* object);

    /**
     * @brief Function used to put an object that draws to the screen into the update loop and draw to the screen
     *
     * @param renderer Pointer to the object that draws to the screen
     */
    void registerRenderer(Renderer* renderer);
    /**
     * @brief function used if the transparency of a renderer changes during runtime
     *
     * @param renderer the renderer that is getting changed
     */
    void updateTransparency(Renderer* renderer);
    void setActiveRenderer(Renderer* renderer, bool active);

    /**
     * @brief The getter for the current active scene
     *
     * @return The current active Scene
     */
    [[nodiscard]] Scene* getActiveScene() const;

    /**
     * @brief Function used to set a new active scene and load all the necessary resources
     *
     * @param name Name of the scene in the resource manager scene map to set as the active scene
     */
    void setActiveScene(const std::string& name);

    /**
     * @brief sets the name of the base scene needed to start the engine
     *
     * @param name Name of the scene in the resource manager scene map
     */
    void setBaseScene(const std::string& name);

    /**
     * @brief Function used for setting the active state of an object and added them to the correct enabled/disabled events
     *
     * @param object Object that the active state is changing on
     * @param active the new active state
     */
    void setObjectActiveState(Object* object, bool active);

    EVENT_ACCESSORS(m_updateEvent)
    EVENT_ACCESSORS(m_destroyEvent)

private:
    /**
     * @brief Starts all other managers preparing to start the game
     *
     * @return 0 if successful and 1 if unsuccessful
     */
    int initialize() override;
    /**
     * @brief Shuts down all other managers and frees necessary data
     */
    void terminate() override;
    /**
     * @brief A setup for the scene called when a scene is loaded to make sure everything can run properly
     */
    void awake();
    /**
     * @brief Caller function for the update event sent to all objects
     */
    void update();
    /**
    * @brief Private default constructor for singleton functionality
    */
    WorldManager() = default;
    /**
    * @brief Private default deconstructor for singleton functionality
    */
    ~WorldManager() override = default;

    void drawPostprocess();

    std::vector<Object*> m_tobeAdded{};
    std::vector<Object*> m_tobeDestroyed{};

    Event<> m_updateEvent{};
    Event<> m_enableEvent{};
    Event<> m_disableEvent{};
    Event<> m_destroyEvent{};

    Event<> m_transparentDrawEvent{};
    Event<> m_opaqueDrawEvent{};

    Event<> m_loadEvent{};
    Event<> m_unloadEvent{};

    std::string m_baseScene{"default"};
    std::string m_activeSceneName{};

    Scene* m_activeScene{nullptr};
};
