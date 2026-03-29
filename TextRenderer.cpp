#include "TextRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Object.h"
#include <iostream>
#include <bits/fs_fwd.h>

#include "glad/glad.h"

TextRenderer::TextRenderer(Object* parent) : Component(parent){
    setFont("assets/defaultAssets/Fonts/arial.ttf");

    material = new Material("assets/defaultAssets/Shaders/defaultUI.vert", "assets/defaultAssets/Shaders/text.frag");
    material->setMaterialType(UI);
    material->setParent(getParent());
    material->setTransparent(true);
    material->addTexture(static_cast<unsigned int>(0));

    color = vec3(1, 1, 1);
}

int TextRenderer::setFont(const char *path) {
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft, path, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

void TextRenderer::update() {

    material->setUniform<vec3>("color", vec3(1,1,1));

    for (int i = 0; i < meshes.size(); ++i) {
        material->setTexture(characters[i].TextureID, 0);

        meshes[i]->draw();
    }

    Component::update();
}

void TextRenderer::setText(const std::string& text) {
    float x = 0, y = 0;

    float maxY = Characters.at(text[0]).Size.y;
    float fullX = 0;
    for (int i = 0; i < text.size(); ++i) {
        const Character character = Characters.at(text[i]);
        fullX += character.Advance >> 6;
    }

    switch (material->getTextAnchorPoint()) {
        case CENTER:
            x = -(fullX / 2.0f);
            y = -(maxY / 2.0f);
            break;
        case LEFT:
            y = -(maxY / 2.0f);
            break;
        case RIGHT:
            x = -fullX;
            y = -(maxY / 2.0f);
            break;
        case TOP:
            x = -(fullX / 2.0f);
            y = -maxY;
            break;
        case BOTTOM:
            x = -(fullX / 2.0f);
            break;
        case TOP_LEFT:
            y = -maxY;
            break;
        case TOP_RIGHT:
            x = -fullX;
            y = -maxY;
            break;
        case BOTTOM_LEFT:
            break;
        case BOTTOM_RIGHT:
            x = fullX;
            break;
        default:
            break;
    }

    characters.clear();
    for (int i = 0; i < text.size(); ++i) {
        Character character = Characters.at(text[i]);

        float xpos = x + character.Bearing.x * 1;
        float ypos = y - (character.Size.y - character.Bearing.y) * 1;

        float w = character.Size.x * 1;
        float h = character.Size.y * 1;

        vector<Mesh::Vertex> vertices {
            Mesh::Vertex(vec3(xpos, ypos, 0), vec3(0, 0, 1), vec2(0, 1)),
            Mesh::Vertex(vec3(xpos + w, ypos, 0), vec3(0, 0, 1), vec2(1, 1)),
            Mesh::Vertex(vec3(xpos + w, ypos + h, 0), vec3(0, 0, 1), vec2(1, 0)),
            Mesh::Vertex(vec3(xpos, ypos + h, 0), vec3(0, 0, 1), vec2(0, 0)),
        };
        vector<unsigned int> indices {
            0, 1, 2,
            0, 2, 3,
        };
        auto m = new Mesh(vertices, indices);
        m->setMaterial(material);
        m->setParent(this);
        meshes.push_back(m);
        characters.push_back(character);

        x += (character.Advance >> 6);
    }
    m_text = text;
}
