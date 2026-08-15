/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once
#include "Component.h"
#include "ResourceManager.h"

/**
 * @brief Component for playing sounds and music in the application
 */
class AudioSourceComponent : public Component{
    friend class ResourceManager;
public:
    /**
     * @brief The constructor that sets the parent object of the component
     *
     * @param parent The parent object the component is attached to
     */
    explicit AudioSourceComponent(Object* parent) : Component(parent){}

    /**
     * @brief The function to play the stored audio sound
     */
    void play();

private:
    /**
     * @brief Runs only once on the first frame it is initiated
     */
    void start() override{}

    /**
     * @brief Runs once every frame it is active
     */
    void update() override;
    /**
     * @brief Runs when the object or component is set to be active
     */
    void enable() override{}
    /**
     * @brief Runs when the object or component is set to be inactive
     */
    void disable() override{}
    /**
     * @brief Runs once when the object or component is being destroyed
     */
    void destroy() override;
    /**
     * @brief Returns a JSON object for this component
     *
     * @return JSON object with all AudioComponent Data
     */
    nlohmann::json toJson() override;
    /**
     * @brief Function that takes a JSON object and builds the component from it
     *
     * @param j JSON with the data for the component
     */
    void fromJson(nlohmann::json j) override;


    /**
     * @brief Helper function to update all the buffers to play the sound
     */
    void update_stream();

    /**
     * @brief The path to the sound file of the loaded sound
     */
    std::string m_soundPath{};
    /**
     * @brief The loaded sound data
     */
    Sound m_sound{};
    /**
     * @brief The source object from OpenAL
     */
    ALuint m_source{};

    /**
     * @brief The OpenAL state of the source object
     */
    ALint m_state{};
    /**
     * @brief The data location to update the buffers for longer play
     */
    std::size_t m_cursor{BUFFER_SIZE * NUM_BUFFERS};
};
