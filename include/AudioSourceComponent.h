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
    explicit AudioSourceComponent(Object3D* parent);

    void play();

private:
    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void destroy() override;

    void update_stream(std::size_t& cursor);

    std::string path{};
    Sound m_sound{};
    ALuint m_source{};

    ALint m_state{};
    std::size_t m_cursor{BUFFER_SIZE * NUM_BUFFERS};
};
