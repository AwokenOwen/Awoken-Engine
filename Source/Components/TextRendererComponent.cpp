//
// Created by AwokenOwen on 7/12/26.
//

#include "TextRendererComponent.h"

#include "Object.h"

void TextRendererComponent::setFont(const std::string& path)
{
    Resource.unloadFont(m_fontPath);
    m_fontPath = path;
    Resource.loadFont(m_fontPath);
}

void TextRendererComponent::start()
{
    constexpr GLfloat vertex_data[] = {
        0.0f,1.0f,
        0.0f,0.0f,
        1.0f,1.0f,
        1.0f,0.0f,
    };

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
    m_text = j["Text"];
    m_fontPath = j["Font"];
    m_materialPath = j["Material"];

    Resource.loadFont(m_fontPath);
    Resource.loadMaterial(m_materialPath);

    m_font = Resource.getFont(m_fontPath);
    m_material = Resource.getMaterial(m_materialPath);
}

void TextRendererComponent::draw()
{
    m_material.load();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_font.m_textureArray);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<Matrix4> transforms{};
    std::vector<int> letterMap{};

    transforms.resize(400);
    letterMap.resize(400);

    float x = getParent()->getWorldPosition().x();
    float y = getParent()->getWorldPosition().y();

    int workingIndex = 0;

    std::string::const_iterator c;
    for (c = m_text.begin(); c != m_text.end(); ++c)
    {
        Character ch = m_font.m_characters.at(*c);

        if (*c == '\n')
        {
            y -= ch.Bearing.y();
            x = getParent()->getWorldPosition().x();
        }else if (*c == ' ')
        {
            x += ch.Advance >> 6;
        }else
        {
            float xpos = x + ch.Bearing.x();
            float ypos = y - (256 - ch.Bearing.y());

            transforms[workingIndex] = Matrix4::Translate(Matrix4(), Vector3(xpos, ypos, 0.0f)) * Matrix4::Scale(Matrix4(), Vector3(256, 256, 0));
            letterMap[workingIndex] = ch.index;

            x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            workingIndex++;
            if (workingIndex == 400 ) {
                drawLimit(workingIndex);
                workingIndex = 0;
            }
        }
    }

    drawLimit(workingIndex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextRendererComponent::drawLimit(int length)
{
    if (length <= 0)
    {
        return;
    }
    //Log.log("%s", m_text.c_str());
}

void TextRendererComponent::drawToShadowMap(LightComponent* light)
{
}
