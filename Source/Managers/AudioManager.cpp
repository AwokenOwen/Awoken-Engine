#include "AudioManager.h"

#include <cstring>
#include "GameManager.h"
#include <iostream>

#include "CameraComponent.h"
#include "Object.h"

int AudioManager::initialize() {
    p_device = alcOpenDevice(nullptr);
    if (!p_device) {
        Log.logError("Error when using alcOpenDevice()");
        return 1;
    }

    p_context = alcCreateContext(p_device, nullptr);
    if (!p_context) {
        Log.logError("Error when using alcCreateContext()");
        return 1;
    }

    alcMakeContextCurrent(p_context);
    if (alcGetError(p_device) != ALC_NO_ERROR ) {
        Log.logError("Error when making context current");
        return 1;
    }

    Log.log("Audio Manager initialized");
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
        setListener(Resource.getMainCamera()->getParent());
    }
    const Vector3 listenerPosition = p_listener->getWorldPosition();
    alListener3f(AL_POSITION, listenerPosition.x, listenerPosition.y, listenerPosition.z);

    for (SourceData source : sources) {
        if (source.state == AL_PLAYING) {
            update_stream(source);
            alGetSourcei(source.source, AL_SOURCE_STATE, &source.state);
            Vector3 sourcePosition = source.p_sourceObject->getWorldPosition();
            alGetSource3f(source.source, AL_POSITION, &sourcePosition.x, &sourcePosition.y, &sourcePosition.z);
        }
    }
}

ALCdevice * AudioManager::getDevice() const {
    return p_device;
}

void AudioManager::setListener(Object *listener) {
    p_listener = listener;
}

void AudioManager::playSource(const int index) const
{
    SourceData sourceData = sources[index];

    alBufferData(sourceData.buffer, sourceData.sound->m_format, sourceData.sound->m_data, sourceData.sound->m_data_size, sourceData.sound->m_sampleRate);

    alGenSources(1, &sourceData.source);
    alSourcei(sourceData.source, AL_BUFFER, sourceData.buffer);

    alSourcePlay(sourceData.source);
}

int AudioManager::registerSource(Object* sourceObject) {
    sources.emplace_back(sourceObject);

    return sources.size() - 1;
}

void AudioManager::setSource(const int index, const std::string& path) const
{
    SourceData sourceData = sources[index];
    sourceData.sound = Resource.getSound(path);
}

void AudioManager::update_stream(const SourceData source)
{
    int cursor = BUFFER_SIZE * NUM_BUFFERS;

    ALint buffersProcessed = 0;
    alGetSourcei(source.source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    if(buffersProcessed <= 0)
        return;

    while(buffersProcessed--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(source.source, 1, &buffer);

        const ALsizei dataSize = BUFFER_SIZE;

        const auto data = new char[dataSize];
        std::memset(data, 0, dataSize);

        std::size_t dataSizeToCopy = BUFFER_SIZE;
        if(cursor + BUFFER_SIZE > source.sound->m_data_size)
            dataSizeToCopy = source.sound->m_data_size - cursor;

        std::memcpy(&data[0], &source.sound->m_data[cursor], dataSizeToCopy);
        cursor += dataSizeToCopy;

        if(dataSizeToCopy < BUFFER_SIZE)
        {
            cursor = 0;
            std::memcpy(&data[dataSizeToCopy], &source.sound->m_data[cursor], BUFFER_SIZE - dataSizeToCopy);
            cursor = BUFFER_SIZE - dataSizeToCopy;
        }

        alBufferData(buffer, source.sound->m_format, data, BUFFER_SIZE, source.sound->m_sampleRate);
        alSourceQueueBuffers(source.source, 1, &buffer);
        delete[] data;
    }
}
