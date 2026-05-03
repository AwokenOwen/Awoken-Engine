//
// Created by awokenowen on 5/3/26.
//

#pragma once
#include "glad/glad.h"
#include <vector>
#include <string>
#include "Math.h"

struct Material {
    unsigned int shader;
    std::vector<unsigned int> textures;

    template<typename T>
    void setUniform(const std::string& location, T value);
};

struct Vertex {
    Vector3 Position;
    Vector3 Normal;
    Vector2 TexCoords;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

class ResourceManager {
};
