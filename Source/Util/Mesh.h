//
// Created by AwokenOwen on 4/24/26.
//

#pragma once
#include <vector>
#include "Math.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};

class Material;
class Mesh {
public:
    explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void draw();
private:
    void setup();

    unsigned int m_VAO{}, m_VBO{}, m_EBO{};

    std::vector<Vertex> m_vertices{};
    std::vector<unsigned int> m_indices{};

    Material* m_material{};
};
