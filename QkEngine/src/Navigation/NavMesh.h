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
	static void RegenerateChunk(const glm::vec3& _startPos, const glm::vec3& _endPos);
	static void CheckCollisions();

	static std::vector<NavMeshNode*> GetPath(const glm::vec3& startPos, const glm::vec3& endPos);
	static std::vector<NavMeshNode*> GetPath(NavMeshNode* startNode, NavMeshNode* endNode);

	static void ShowNavMesh();
	static void DebugDraw();

private:
	static NavMeshNode* GetNodeFromPosition(const glm::vec3& position);
	static NavMeshNode* GetNodeAt2DIndex(const int& x, const int& y);
	static std::vector<NavMeshNode*> GetNeighbours(const NavMeshNode const* node);

	static std::vector<NavMeshNode*> RetracePath(NavMeshNode* startNode, NavMeshNode* endNode);
	static int GetDistance(const NavMeshNode* nodeA, const NavMeshNode* nodeB);

	static std::vector<NavMeshNode> nodes;

	static glm::vec3 startPos;
	static int width;
	static float nodeSize;

	static bool showNavMesh;
};

