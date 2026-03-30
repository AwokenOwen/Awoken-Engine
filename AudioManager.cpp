#include "AudioManager.h"
#include "ResourceManager.h"
#include <iostream>
#include "Object.h"
#include "AL/al.h"


int AudioManager::initialize() {
    p_device = alcOpenDevice(nullptr);
    if (!p_device) {
        std::cout <<"Error when using alcOpenDevice() "<< std::endl;
        return -1;
    }

    p_context = alcCreateContext(p_device, nullptr);
    if (!p_context) {
        std::cout <<"Error when using alcCreateContext() "<< std::endl;
    }

    alcMakeContextCurrent(p_context);
    if (alcGetError(p_device) != ALC_NO_ERROR ) {
        std::cout << "Error when making context current" << std::endl;
    }

    return 0;
}

void AudioManager::terminate() const {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(p_context);
    alcCloseDevice(p_device);
}

AudioManager & AudioManager::getInstance() {
    static AudioManager single;
    return single;
}

void AudioManager::update() const {
    vec3 position = p_listener->getLocalPosition();
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

ALCdevice * AudioManager::getDevice() const {
    return p_device;
}

void AudioManager::setListener(Object *listener) {
    p_listener = listener;
}

void AudioManager::PlaySoundTest() {
    auto sound = Resource.loadSound("assets/defaultAssets/Sounds/melee-sound-mono.wav");

    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALenum format;
    if(sound->m_channels == 1 && sound->m_bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if(sound->m_channels  == 1 && sound->m_bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if(sound->m_channels  == 2 && sound->m_bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if(sound->m_channels  == 2 && sound->m_bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << sound->m_channels << " channels, "
            << sound->m_bitsPerSample << " bps" << std::endl;
        return;
    }

    alBufferData(buffer, format, sound->m_data, sound->m_data_size, sound->m_sampleRate);

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
}

AudioManager::AudioManager() = default;
