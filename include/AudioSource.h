/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once
#include "Component.h"

class AudioSource : public Component{
public:
    explicit AudioSource(Object* parent);

    void setSound(const std::string& path);

    void play();
private:
    int m_sourceId;
};
