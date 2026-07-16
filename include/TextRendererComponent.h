//
// Created by AwokenOwen on 7/12/26.
//

#pragma once
#include "Component.h"
#include "Renderer.h"
#include "ResourceManager.h"

struct InstanceData
{
    Matrix4 transform{};
    int letterIndex{};
};

class TextRendererComponent : public Component, public Renderer
{
    friend class ResourceManager;
public:
    explicit TextRendererComponent(Object* parent)
        : Component(parent)
    {
    }

    void setFont(const std::string& path);
    void setText(const std::string& text);

private:
    void init();
    void start() override;
    void update() override;
    void enable() override;
    void disable() override;
    void destroy() override;
    nlohmann::json toJson() override;
    void fromJson(nlohmann::json j) override;
    void draw() override;
    void drawToShadowMap(LightComponent* light) override;

    std::string m_fontPath{"assets/defaultAssets/Fonts/arial.ttf"};
    Font m_font;
    std::string m_materialPath{"assets/defaultAssets/Materials/text.json"};
    Material m_material{};
    std::string m_text{"Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos."};

    int capacity{1024};
    float xSize{200.f};
    float ySize{200.f};
    std::vector<InstanceData> m_instanceData{};
    unsigned int VAO{}, VBO{}, instanceVBO{};
};

