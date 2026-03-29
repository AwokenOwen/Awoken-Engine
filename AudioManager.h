/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once
#include "AL/alc.h"

/**
 * @brief Singleton Macro
 */
#define Audio AudioManager::getInstance()

/**
 * @brief Forward Declaration of Object
 */
class Object;
/**
 * @brief Manager that handles Audio device
 */
class AudioManager {
public:
    /**
     * @brief Starts up Audio manager and return 0 if successful and 1 if failed
     *
     * @return int
     */
    int initialize();
    /**
     * @brief Shut Down the audio manager freeing all data
     */
    void terminate() const;
    /**
     * @brief AudioManager Singleton get function
     *
     * @return static AudioManager Instance
     */
    static AudioManager& getInstance();

    void update() const;

    /**
     * @brief Getter for the current device
     *
     * @return ALC device*
     */
    [[nodiscard]] ALCdevice* getDevice() const;

    void setListener(Object* listener);

    void PlaySoundTest();
private:
    AudioManager();

    ALCdevice* p_device{};
    ALCcontext* p_context{};

    Object* p_listener{};
};