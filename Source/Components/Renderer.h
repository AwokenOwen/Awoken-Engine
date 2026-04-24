//
// Created by awokenowen on 4/22/26.
//

#pragma once
#include "WorldManager.h"

class Renderer {
public:
    Renderer(bool isTransparent);
    virtual ~Renderer() = default;
    virtual void draw() = 0;
    [[nodiscard]] bool getTransparency();
    void setTransparency(bool transparency);
private:
    bool m_transparent;
};

inline Renderer::Renderer(bool isTransparent) {
    m_transparent = isTransparent;
    World.registerRenderer(this);
}

inline bool Renderer::getTransparency() {
    return m_transparent;
}

inline void Renderer::setTransparency(bool transparency) {
    m_transparent = transparency;
    if (m_transparent != transparency) {
        World.updateTransparency(this);
    }
}
