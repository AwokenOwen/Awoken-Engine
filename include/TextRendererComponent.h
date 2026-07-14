//
// Created by AwokenOwen on 7/12/26.
//

#pragma once
#include "Component.h"
#include "Renderer.h"
#include "ResourceManager.h"

class TextRendererComponent : public Component, public Renderer
{
    friend class ResourceManager;
public:
    explicit TextRendererComponent(Object* parent)
        : Component(parent)
    {
    }

    void setFont(const std::string& path);

private:
    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    void destroy() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void draw() override;
    void drawLimit(int length);
    void drawToShadowMap(LightComponent* light) override;

    std::string m_fontPath{"assets/defaultAssets/Fonts/arial.ttf"};
    Font m_font;
    std::string m_materialPath{"assets/defaultAssets/Materials/text.json"};
    Material m_material{};
    std::string m_text{"Hello World!"};

    unsigned int VAO{}, VBO{};
};

