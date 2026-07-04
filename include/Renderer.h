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

protected:
    void addToDraw();
    void removeFromDraw();
    void addToShadowDraw();
    void removeFromShadowDraw();

private:
    virtual void draw() = 0;
    virtual void drawToShadowMap(LightComponent* light) = 0;
    bool m_transparent{};
    bool inDraw{};
    bool inShadowDraw{};
};

inline bool Renderer::getTransparency() const {
    return m_transparent;
}

inline void Renderer::setTransparency(const bool transparency) {
    const bool draw = inDraw;
    const bool shadow = inShadowDraw;

    if (draw) removeFromDraw();
    if (shadow) removeFromShadowDraw();

    m_transparent = transparency;

    if (draw) addToDraw();
    if (shadow) addToShadowDraw();
}

inline void Renderer::addToDraw()
{
    m_transparent ? World.addTransparentDrawEvent(this, &Renderer::draw) : World.addOpaqueDrawEvent(this, &Renderer::draw);
    inDraw = true;
}

inline void Renderer::removeFromDraw()
{
    m_transparent ? World.removeTransparentDrawEvent(this, &Renderer::draw) : World.removeOpaqueDrawEvent(this, &Renderer::draw);
    inDraw = false;
}

inline void Renderer::addToShadowDraw()
{
    inShadowDraw = true;
    if (m_transparent)
    {
        return;
    }
    World.addShadowMapDrawEvent(this, &Renderer::drawToShadowMap);
}

inline void Renderer::removeFromShadowDraw()
{
    inShadowDraw = false;
    if (m_transparent)
    {
        return;
    }
    World.removeShadowMapDrawEvent(this, &Renderer::drawToShadowMap);
}
