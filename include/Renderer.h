//
// Created by AwokenOwen on 4/22/26.
//

#pragma once
#include "WorldManager.h"

class Renderer {
    friend class WorldManager;
public:
    explicit Renderer() = default;
    virtual ~Renderer() = default;
    [[nodiscard]] bool getTransparency() const;
    void setTransparency(bool transparency);
    void registerRenderer(bool transparency);

    virtual void load() = 0;
    virtual void unload() = 0;
private:
    virtual void draw() = 0;
    bool m_transparent{};
};

inline bool Renderer::getTransparency() const {
    return m_transparent;
}

inline void Renderer::setTransparency(const bool transparency) {
    m_transparent = transparency;
    if (m_transparent != transparency) {
        World.updateTransparency(this);
    }
}

inline void Renderer::registerRenderer(const bool transparency)
{
    m_transparent = transparency;
    World.registerRenderer(this);
}
