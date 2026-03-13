#pragma once
#include "Component.h"
#include <glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    long Advance;    // Offset to advance to next glyph
};

class TextRenderer : public Component{
public:
    TextRenderer(Object* parent);
    int setFont(const char* path);
    void update() override;
private:
    std::map<char, Character> Characters;
    FT_Library ft{};
    FT_Face face{};
};