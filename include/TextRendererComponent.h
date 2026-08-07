//
// Created by AwokenOwen on 7/12/26.
//

#pragma once
#include "Component.h"
#include "ResourceManager.h"

struct InstanceData
{
    Matrix4 transform{};
    int letterIndex{};
};

class TextRendererComponent : public Component
{
    friend class ResourceManager;
public:
    explicit TextRendererComponent(Object* parent)
        : Component(parent)
    {
    }

    void setFont(const std::string& path);
    void setText(const std::string& text);
    void setBoxSize(const Vector2& size);

private:
    void init();
    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    void destroy() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void draw();
    void drawToShadowMap(LightComponent* light);

    std::string m_fontPath{"assets/defaultAssets/Fonts/arial.ttf"};
    Font m_font;
    std::string m_materialPath{"assets/defaultAssets/Materials/text.json"};
    Material m_material{};
    std::string m_text{"Hello World!"};
    int capacity{1024};
    Vector2 boxSize{256, 0};
    std::vector<InstanceData> m_instanceData{};
    unsigned int VAO{}, VBO{}, instanceVBO{};
};

