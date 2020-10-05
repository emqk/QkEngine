#pragma once

#include "NavMeshNode.h"
#include <vector>
#include <glm\ext\vector_float3.hpp>

class NavMesh
{
public:
	NavMesh() = delete;
	~NavMesh() = delete;

	static void Generate(const glm::vec3& _startPos, const int& _width, const float& _nodeSize);
	static void CheckCollisions();

	static void ShowNavMesh();
	static void DebugDraw();

private:

	static std::vector<NavMeshNode> nodes;

	static glm::vec3 startPos;
	static int width;
	static float nodeSize;
};

