//
// Created by awokenowen on 4/13/26.
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
class CameraComponent;

/**
 * @brief Singleton class that handles all Frame-buffers and the World events
 */
class WorldManager : public Manager{
    friend class GameManager;
public:
    /**
     * @brief Getter for the singleton instance
     *
     * @return The singleton instance of the GameManager
     */
    static WorldManager& getInstance();

    /**
     * @brief Function used to put a new object into the update loop
     *
     * @param object The new Object needed to be added
     * @param parent The Object the new object will be parented to, nullptr if root object
     */
    void registerObject(Object* object, const Object* parent = nullptr);
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

    /**
     * @brief Getter for the current active camera in the current active scene
     *
     * @return The Camera Component of the current active camera
     */
    CameraComponent* getActiveCamera();

    /**
     * @brief The getter for the current active scene
     *
     * @return The current active Scene
     */
    Scene* getActiveScene() const;

    /**
     * @brief Function that grabs a scene json file and loads it into memory
     *
     * @param path
     */
    void loadScene(const char* path);
    void setActiveScene(const char* path, bool isFile = false);
    void setBaseScene(const char* name);
    void setObjectActiveState(Object* object, bool active);
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

    static Scene* createSceneFromFile(const char* path);

    std::vector<Object*> m_tobeAdded{};
    std::vector<Object*> m_tobeDestroyed{};

    std::string m_baseScene{"default"};
    Scene* m_activeScene{};
    std::map<std::string, Scene*> m_loadedScenes{};
};

struct Scene {
    std::string name;

    Event<> m_updateEvent{&World};
    Event<> m_enableEvent{&World};
    Event<> m_disableEvent{&World};
    Event<> m_destroyEvent{&World};

    Event<> m_transparentDrawEvent{&World};
    Event<> m_opaqueDrawEvent{&World};

    std::vector<Object*> m_rootObjects{};

    nlohmann::json toJson() const;

    void end() const;
};
