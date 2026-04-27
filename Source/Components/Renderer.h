//
// Created by AwokenOwen on 4/22/26.
//

#pragma once
#include "WorldManager.h"

class Renderer {
    friend class WorldManager;
public:
    explicit Renderer(bool isTransparent);
    virtual ~Renderer() = default;
    [[nodiscard]] bool getTransparency() const;
    void setTransparency(bool transparency);
private:
    virtual void draw() = 0;
    bool m_transparent;
};

inline Renderer::Renderer(const bool isTransparent) {
    m_transparent = isTransparent;
    World.registerRenderer(this);
}

inline bool Renderer::getTransparency() const {
    return m_transparent;
}

inline void Renderer::setTransparency(const bool transparency) {
    m_transparent = transparency;
    if (m_transparent != transparency) {
        World.updateTransparency(this);
    }
}
