#include "MeshNew.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Gizmos.h"
#include "ResourceManager.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const Bounds& _bounds, const std::string& _name)
{
    this->vertices = vertices;
    this->indices = indices;
    bounds = _bounds;
    name = _name;
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return vertices;
}

const std::vector<unsigned int>& Mesh::GetIndices() const
{
    return indices;
}

const Bounds& Mesh::GetBounds() const
{
    return bounds;
}