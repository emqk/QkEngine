#pragma once
#include <vector>
#include <string>
#include "Bounds.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

    std::string name;

    std::vector<float>& GetVertices();
    std::vector<unsigned int>& GetIndices();
    Bounds& GetBounds();

    void SetVertices(const std::vector<float>& verts);
    void SetIndices(const std::vector<unsigned int>& inds);
    void SetBounds(const Bounds& _bounds);

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Bounds bounds;
};