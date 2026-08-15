 #include "AudioSourceComponent.h"

#include "LogManager.h"
#include "ResourceManager.h"

void AudioSourceComponent::play() {
    // Set mix values to the audio source
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSource3f(m_source, AL_POSITION, 0, 0,0);
    alSource3f(m_source, AL_VELOCITY, 0, 0,0);
    alSourcei(m_source, AL_LOOPING, AL_FALSE);

    // Queue the sound data into the buffers
    alSourceQueueBuffers(m_source, NUM_BUFFERS, &m_sound.m_ID[0]);

    // Play the sound
    alSourcePlay(m_source);

    // Error check
    if(const auto error = alGetError(); error != AL_NO_ERROR)
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

    // Set the state to playing
    m_state = AL_PLAYING;
}

void AudioSourceComponent::update()
{
    // If the state is playing
    if (m_state == AL_PLAYING)
    {
        // Update the buffers
        update_stream();
        // Update the state
        alGetSourcei(m_source, AL_SOURCE_STATE, &m_state);
    }
}

nlohmann::json AudioSourceComponent::toJson()
{
    // Make a JSON object
    auto j = nlohmann::json();

    // Set the type to Audio Source
    j["Type"] = "AudioSource";

    // Set Sound to be the path to the sound file
    j["Sound"] = m_soundPath;

    // Add the other values as well

    // Return the JSON
    return j;
}

void AudioSourceComponent::fromJson(nlohmann::json j)
{
    // Grab the sound file
    m_soundPath = j["Sound"].get<std::string>();

    // Load and get the sound file
    Resource.loadSound(m_soundPath);
    m_sound = Resource.getSound(m_soundPath);

    // Generate our source
    alGenSources(1, &m_source);
}

void AudioSourceComponent::destroy()
{
    // Delete the source from memory
    alDeleteBuffers(1, &m_source);

    // Tell the ResourceManager this no longer needs the sound
    Resource.unloadSound(m_soundPath);
}

void AudioSourceComponent::update_stream() {
    // Get the buffers that need to be processed from the source
    ALint buffersProcessed = 0;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    // If it's 0 or less return nothing to do
    if(buffersProcessed <= 0)
        return;

    // While buffers need to be processed
    while(buffersProcessed--)
    {
        // Grab the next buffer
        ALuint buffer;
        alSourceUnqueueBuffers(m_source, 1, &buffer);

        // Set the size to be the buffer size
        constexpr ALsizei dataSize = BUFFER_SIZE;

        // Make a char array from the heap and set it all to 0
        const auto data = new char[dataSize];
        std::memset(data, 0, dataSize);

        // Get the right amount of data necessary to copy from the sound data
        std::size_t dataSizeToCopy = BUFFER_SIZE;
        // If there is less data needed to be processed that BUFFER_SIZE then only copy the amount needed to finish the data
        if(m_cursor + BUFFER_SIZE > m_sound.m_soundData.size())
            dataSizeToCopy = m_sound.m_soundData.size() - m_cursor;

        // Copy the data from the sound data over to the array
        std::memcpy(&data[0], &m_sound.m_soundData[m_cursor], dataSizeToCopy);
        m_cursor += dataSizeToCopy;

        // Make sure to grab the beginning of the data and fit it at the end for looping if activated
        if(dataSizeToCopy < BUFFER_SIZE)
        {
            m_cursor = 0;
            std::memcpy(&data[dataSizeToCopy], &m_sound.m_soundData[m_cursor], BUFFER_SIZE - dataSizeToCopy);
            m_cursor = BUFFER_SIZE - dataSizeToCopy;
        }

        // Update the buffer data with the data grabbed
        alBufferData(buffer, m_sound.m_format, data, BUFFER_SIZE, m_sound.m_sampleRate);
        // Requeue the buffer to be played
        alSourceQueueBuffers(m_source, 1, &buffer);

        /// Delete the data from the heap to prevent mem leak
        delete[] data;
    }
}
