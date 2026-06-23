/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once

#include <vector>

#include "ResourceManager.h"

/**
 * @brief Singleton Macro
 */
#define Audio AudioManager::getInstance()

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

/**
 * @brief Forward Declaration of Object
 */
class Object;
struct SourceData {
    ALuint source{};
    ALuint buffer{};
    ALint state{};
    SoundData sound{};
    Object* p_sourceObject{};

    explicit SourceData(Object* sourceObject)
    {
        alGenSources(1, &source);
        alGenBuffers(NUM_BUFFERS, &buffer);
        state = AL_STOPPED;
        p_sourceObject = sourceObject;
    }
};
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

    void update();

    /**
     * @brief Getter for the current device
     *
     * @return ALC device*
     */
    [[nodiscard]] ALCdevice* getDevice() const;

    void setListener(Object* listener);

    void playSource(int index) const;

    int registerSource(Object* sourceObject);

    void setSource(int index, const std::string& path) const;
private:
    AudioManager() = default;

    ALCdevice* p_device{};
    ALCcontext* p_context{};

    Object* p_listener{};

    std::vector<SourceData> sources;

    void update_stream(const SourceData source);
};