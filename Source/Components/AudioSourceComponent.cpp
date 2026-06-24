#include "AudioSourceComponent.h"
#include "AudioManager.h"

AudioSourceComponent::AudioSourceComponent(Object *parent) : Component(parent)
{
}

void AudioSourceComponent::play() {
    Audio.playSource(m_sourceId);
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
    m_sourceId = Audio.registerSource(getParent());

    path = j["Sound"].get<std::string>();

    Resource.loadSound(path);
    Audio.setSource(m_sourceId, path);
}
