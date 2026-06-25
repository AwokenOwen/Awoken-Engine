//
// Created by AwokenOwen on 6/24/26.
//

#include "AudioManager.h"

#include "LogManager.h"

AudioManager& AudioManager::getInstance()
{
    static AudioManager single;
    return single;
}

int AudioManager::initialize()
{
    m_device = alcOpenDevice(nullptr);
    if (!m_device)
    {
        Log.logError("Could not find a default audio device.");
        return 1;
    }
    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context)
    {
        Log.logError("Could not create a audio context.");
        return 1;
    }
    alcMakeContextCurrent(m_context);
    if (alcGetError(m_device) != ALC_NO_ERROR )
    {
        Log.logError("Could not make context the current context.");
        return 1;
    }

    Log.log("AudioManager initialized");
    return 0;
}

void AudioManager::terminate()
{
    alcCloseDevice(m_device);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_context);

    Log.log("AudioManager terminated");
}
