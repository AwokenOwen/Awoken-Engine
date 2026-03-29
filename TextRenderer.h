#pragma once
#include "Component.h"
#include <glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <string>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    long Advance;    // Offset to advance to next glyph
};

class Mesh;
class Material;
class TextRenderer : public Component{
public:
    TextRenderer(Object* parent);
    int setFont(const char* path);
    void update() override;
    void setText(const std::string& text);
private:
    std::map<char, Character> Characters;
    FT_Library ft{};
    FT_Face face{};

    Material* material{};

    std::vector<Mesh*> meshes{};
    std::vector<Character> characters{};

    std::string m_text;
    glm::vec3 color{};
};