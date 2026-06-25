#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "ResourceManager.h"

AudioSourceComponent::AudioSourceComponent(Object *parent) : Component(parent)
{
}

void AudioSourceComponent::play() {
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSource3f(m_source, AL_POSITION, 0, 0,0);
    alSource3f(m_source, AL_VELOCITY, 0, 0,0);
    alSourcei(m_source, AL_LOOPING, AL_TRUE);
    alSourcei(m_source, AL_BUFFER, m_sound.m_ID);

    alSourcePlay(m_source);

    m_state = AL_PLAYING;
}

void AudioSourceComponent::start()
{
    play();
}

void AudioSourceComponent::update()
{

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

    alGenBuffers(1, &m_source);
}

void AudioSourceComponent::destroy()
{
    alDeleteBuffers(1, &m_source);
    delete this;
}
