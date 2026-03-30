#include "AudioManager.h"

#include <cstring>

#include "ResourceManager.h"
#include "GameManager.h"
#include <iostream>
#include "Object.h"
#include "Camera.h"
#include "Scene.h"
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

void AudioManager::update() {
    if (p_listener == nullptr) {
        setListener(Game.getActiveScene()->getCamera());
    }
    vec3 position = p_listener->getLocalPosition();
    alListener3f(AL_POSITION, position.x, position.y, position.z);

    if (state == AL_PLAYING) {
        update_stream();
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    }
}

ALCdevice * AudioManager::getDevice() const {
    return p_device;
}

void AudioManager::setListener(Object *listener) {
    p_listener = listener;
}

void AudioManager::PlaySoundTest() {
    sound = Resource.loadSound("assets/defaultAssets/Sounds/Rivals.wav");

    ALuint buffer;
    alGenBuffers(NUM_BUFFERS, &buffer);

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

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
}

AudioManager::AudioManager() = default;

void AudioManager::update_stream()
{
    int cursor = BUFFER_SIZE * NUM_BUFFERS;

    ALint buffersProcessed = 0;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    if(buffersProcessed <= 0)
        return;

    while(buffersProcessed--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(source, 1, &buffer);

        ALsizei dataSize = BUFFER_SIZE;

        char* data = new char[dataSize];
        std::memset(data, 0, dataSize);

        std::size_t dataSizeToCopy = BUFFER_SIZE;
        if(cursor + BUFFER_SIZE > sound->m_data_size)
            dataSizeToCopy = sound->m_data_size - cursor;

        std::memcpy(&data[0], &sound->m_data[cursor], dataSizeToCopy);
        cursor += dataSizeToCopy;

        if(dataSizeToCopy < BUFFER_SIZE)
        {
            cursor = 0;
            std::memcpy(&data[dataSizeToCopy], &sound->m_data[cursor], BUFFER_SIZE - dataSizeToCopy);
            cursor = BUFFER_SIZE - dataSizeToCopy;
        }

        alBufferData(buffer, format, data, BUFFER_SIZE, sound->m_sampleRate);
        alSourceQueueBuffers(source, 1, &buffer);
        delete[] data;
    }
}
