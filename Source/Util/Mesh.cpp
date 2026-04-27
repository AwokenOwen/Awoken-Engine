//
// Created by awokenowen on 4/24/26.
//

#include "Mesh.h"
#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->m_vertices = std::move(vertices);
    this->m_indices = std::move(indices);
}

void Mesh::draw() {
}

void Mesh::setup() {

}
