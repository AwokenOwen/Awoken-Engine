//
// Created by AwokenOwen on 7/12/26.
//

#include <sstream>

#include "TextRendererComponent.h"

#include "CameraComponent.h"
#include "Object.h"
#include "WindowManager.h"

void TextRendererComponent::setFont(const std::string& path)
{
    Resource.unloadFont(m_fontPath);
    m_fontPath = path;
    Resource.loadFont(m_fontPath);
}

void TextRendererComponent::setText(const std::string& text)
{
    glBindVertexArray(VAO);
    auto characters = text;
    m_text = text;
    std::erase(characters, ' ');
    std::erase(characters, '\n');

    if (characters.size() > capacity)
    {
        while (characters.size() > capacity)
        {
            capacity *= 2;
        }
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                reinterpret_cast<void*>(offsetof(InstanceData, transform) + sizeof(Vector4) * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 1, GL_INT, sizeof(InstanceData),
            reinterpret_cast<void*>(offsetof(InstanceData, letterIndex)));

        glVertexAttribDivisor(5, 1);
    }

    float yLine = std::numeric_limits<float>::min();
    std::string::const_iterator c;
    long xSpace{};
    for (c = m_text.begin(); c != m_text.end(); ++c)
    {
        yLine = std::max(yLine, m_font.m_characters[*c].Size.y());
        if (*c==' ')
        {
            xSpace = m_font.m_characters[*c].Advance;
        }
    }

    std::istringstream iss(m_text);
    std::vector<std::string> words;
    std::string w;

    while (iss >> w) {
        words.push_back(w);
    }

    // Edit these to fit anchor points
    Vector2 anchorValues{};
    anchorValues[0] = -xSize / 2.f;
    anchorValues[1] = ySize / 2.f;

    constexpr int copyX{};
    int x = copyX;
    int y{};

    for (auto word : words)
    {
        for (c = word.begin(); c != word.end(); ++c)
        {
            auto [index, Size, Bearing, Advance] = m_font.m_characters[*c];

            if (*c == '\n') {
                y -= yLine * 1.3f;
                x = copyX;
            }
            else
            {
                float xPos = x + Bearing.x();
                float yPos = y - (256 - Bearing.y());
                InstanceData instanceData{};
                instanceData.transform = Matrix4::Translate({}, {xPos + anchorValues.x(), yPos + anchorValues.y(), 0}) * Matrix4::Scale({}, {256, 256, 0});
                instanceData.letterIndex = index;
                m_instanceData.push_back(instanceData);

                x+= (Advance >> 6);
            }
        }
        if (x + (xSpace >> 6) > xSize)
        {
            y -= yLine * 1.3f;
            x = copyX;
        }else
        {
            x += (xSpace >> 6);
        }
    }

    glBufferSubData( GL_ARRAY_BUFFER, 0, m_instanceData.size() * sizeof(InstanceData), m_instanceData.data());
    glBindVertexArray(0);
}

void TextRendererComponent::init()
{
    setTransparency(true);
    const std::vector vertices
    {
        Vector2(0.f, 1.f),
        Vector2(0.f, 0.f),
        Vector2(1.f, 1.f),
        Vector2(1.f, 0.f)
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), static_cast<void*>(nullptr));

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(1 + i);
        glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
            reinterpret_cast<void*>(offsetof(InstanceData, transform) + sizeof(Vector4) * i));
        glVertexAttribDivisor(1 + i, 1);
    }

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(InstanceData),
        reinterpret_cast<void*>(offsetof(InstanceData, letterIndex)));

    glVertexAttribDivisor(5, 1);
    glBindVertexArray(0);
}

void TextRendererComponent::start()
{

}

void TextRendererComponent::update()
{
    addToDraw();
}

void TextRendererComponent::enable()
{
}

void TextRendererComponent::disable()
{
}

void TextRendererComponent::destroy()
{
}

nlohmann::json TextRendererComponent::toJson()
{
    nlohmann::json j{};

    j["Type"] = "TextRenderer";
    j["Text"] = m_text;
    j["Font"] = m_fontPath;
    j["Material"] = m_materialPath;

    return j;
}

void TextRendererComponent::fromJson(nlohmann::json j)
{
    init();
    m_fontPath = j["Font"];
    m_materialPath = j["Material"];

    Resource.loadFont(m_fontPath);
    Resource.loadMaterial(m_materialPath);

    m_font = Resource.getFont(m_fontPath);
    m_material = Resource.getMaterial(m_materialPath);
    setText(j["Text"]);
}

void TextRendererComponent::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_material.load();
    m_material.setUniform("projection", Resource.getMainCamera()->getOrthographicMatrix());
    m_material.setUniform("model", getParent()->getWorldMatrix());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_font.m_textureArray);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_instanceData.size());

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glDisable(GL_CULL_FACE);
}


void TextRendererComponent::drawToShadowMap(LightComponent* light)
{
}