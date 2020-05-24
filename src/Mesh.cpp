#include "Mesh.h"
#include <iostream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::vector<float>& Mesh::GetVertices()
{
	return vertices;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
	return indices;
}

Bounds& Mesh::GetBounds()
{
    return bounds;
}

void Mesh::SetVertices(const std::vector<float>& verts)
{
    vertices = verts;
}

void Mesh::SetIndices(const std::vector<unsigned int>& inds)
{
	indices = inds;
}

void Mesh::SetBounds(const Bounds& _bounds)
{
    bounds = _bounds;
}