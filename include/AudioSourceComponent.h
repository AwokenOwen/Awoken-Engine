/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once
#include "Component.h"
#include "ResourceManager.h"

class AudioSourceComponent : public Component{
    friend class ResourceManager;
public:
    explicit AudioSourceComponent(Object* parent);

    void play();

private:
    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void destroy() override;

    std::string path{"assets/defaultAssets/Sounds/Rivals_Test.wav"};
    Sound m_sound{};
    ALuint m_source{};

    ALint m_state{};
};
