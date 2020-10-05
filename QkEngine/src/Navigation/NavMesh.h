#pragma once

#include <vector>
#include <glm\ext\vector_float3.hpp>

class NavMesh
{
public:
	NavMesh() = delete;
	~NavMesh() = delete;

	static void Generate(const glm::vec3& _startPos, const int& _width, const float& _nodeSize);

	static void ShowNavMesh();
	static void DebugDraw();

private:

	static std::vector<glm::vec3> nodesLocation;

	static glm::vec3 startPos;
	static int width;
	static float nodeSize;
};

