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

class LightComponent;
struct Scene;
class Renderer;
class Object3D;

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
    Object3D* instantiateObject(Object3D* parent = nullptr);

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

    EVENT_ACCESSORS(UpdateEvent)
    EVENT_ACCESSORS(EnableEvent)
    EVENT_ACCESSORS(DisableEvent)
    EVENT_ACCESSORS(DestroyEvent)
    EVENT_ACCESSORS(TransparentDrawEvent)
    EVENT_ACCESSORS(OpaqueDrawEvent)
    EVENT_ACCESSORS(ShadowMapDrawEvent, LightComponent*)
    EVENT_ACCESSORS(LoadEvent)
    EVENT_ACCESSORS(UnloadEvent)

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

    std::vector<Object3D*> m_tobeAdded{};
    std::vector<Object3D*> m_tobeDestroyed{};

    Event<> UpdateEvent{};
    Event<> EnableEvent{};
    Event<> DisableEvent{};
    Event<> DestroyEvent{};

    Event<> TransparentDrawEvent{};
    Event<> OpaqueDrawEvent{};

    Event<LightComponent*> ShadowMapDrawEvent{};

    Event<> LoadEvent{};
    Event<> UnloadEvent{};

    std::string m_baseScene{"default"};
    std::string m_activeSceneName{};

    Scene* m_activeScene{nullptr};

    std::vector<LightComponent*> m_lights{};
};
