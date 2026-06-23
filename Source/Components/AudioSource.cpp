#include "AudioSource.h"
#include "AudioManager.h"

AudioSource::AudioSource(Object *parent) : Component(parent){
    m_sourceId = Audio.registerSource(getParent());
}

void AudioSource::setSound(const std::string& path) {
    Resource.loadSound(path);
    Audio.setSource(m_sourceId, path);
}

void AudioSource::play() {
    Audio.playSource(m_sourceId);
}
