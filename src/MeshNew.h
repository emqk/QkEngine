#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Bounds.h"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const Bounds& _bounds, const std::string& _name);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;
    const Bounds& GetBounds() const;

    std::string name;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Bounds bounds;
};