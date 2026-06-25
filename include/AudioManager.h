//
// Created by AwokenOwen on 6/24/26.
//
#pragma once
#include "Manager.h"
#include "AL/alc.h"

#define Audio AudioManager::getInstance()

class AudioManager : public Manager
{
    friend class GameManager;
public:
    static AudioManager& getInstance();
private:
    AudioManager() = default;
    int initialize() override;
    void terminate() override;

    ALCdevice* m_device{};
    ALCcontext* m_context{};
};
