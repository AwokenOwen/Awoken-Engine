#include "AudioSourceComponent.h"

#include <format>

#include "LogManager.h"
#include "ResourceManager.h"

AudioSourceComponent::AudioSourceComponent(Object *parent) : Component(parent)
{
}

void AudioSourceComponent::play() {
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSource3f(m_source, AL_POSITION, 0, 0,0);
    alSource3f(m_source, AL_VELOCITY, 0, 0,0);
    alSourcei(m_source, AL_LOOPING, AL_FALSE);

    alSourceQueueBuffers(m_source, NUM_BUFFERS, &m_sound.m_ID[0]);

    alSourcePlay(m_source);

    auto error = alGetError();
    if(error != AL_NO_ERROR)
    {
        switch(error)
        {
        case AL_INVALID_NAME:
            Log.logError("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
            break;
        case AL_INVALID_ENUM:
            Log.logError("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
            break;
        case AL_INVALID_VALUE:
            Log.logError("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case AL_INVALID_OPERATION:
            Log.logError("AL_INVALID_OPERATION: the requested operation is not valid");
            break;
        case AL_OUT_OF_MEMORY:
            Log.logError("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            Log.logError("UNKNOWN AL ERROR");
        }
    }

    m_state = AL_PLAYING;
}

void AudioSourceComponent::start()
{

}

void AudioSourceComponent::update()
{
    if (m_state == AL_PLAYING)
    {
        update_stream(m_cursor);
        alGetSourcei(m_source, AL_SOURCE_STATE, &m_state);
    }
}

void AudioSourceComponent::enable()
{
}

void AudioSourceComponent::disable()
{
}

nlohmann::json AudioSourceComponent::toJson()
{
    auto j = nlohmann::json();

    j["Type"] = "AudioSource";

    j["Sound"] = path;

    return j;
}

void AudioSourceComponent::fromJson(nlohmann::json j)
{
    path = j["Sound"].get<std::string>();

    Resource.loadSound(path);
    m_sound = Resource.getSound(path);

    alGenSources(1, &m_source);
}

void AudioSourceComponent::destroy()
{
    alDeleteBuffers(1, &m_source);
    delete this;
}

void AudioSourceComponent::update_stream(std::size_t& cursor)
{
    ALint buffersProcessed = 0;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    if(buffersProcessed <= 0)
        return;

    while(buffersProcessed--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(m_source, 1, &buffer);

        ALsizei dataSize = BUFFER_SIZE;

        char* data = new char[dataSize];
        std::memset(data, 0, dataSize);

        std::size_t dataSizeToCopy = BUFFER_SIZE;
        if(cursor + BUFFER_SIZE > m_sound.m_soundData.size())
            dataSizeToCopy = m_sound.m_soundData.size() - cursor;

        std::memcpy(&data[0], &m_sound.m_soundData[cursor], dataSizeToCopy);
        cursor += dataSizeToCopy;

        if(dataSizeToCopy < BUFFER_SIZE)
        {
            cursor = 0;
            std::memcpy(&data[dataSizeToCopy], &m_sound.m_soundData[cursor], BUFFER_SIZE - dataSizeToCopy);
            cursor = BUFFER_SIZE - dataSizeToCopy;
        }

        alBufferData(buffer, m_sound.m_format, data, BUFFER_SIZE, m_sound.m_sampleRate);
        alSourceQueueBuffers(m_source, 1, &buffer);

        delete[] data;
    }
}
