#pragma once
#include <glm\ext\vector_float3.hpp>

class NavMeshNode
{
public:
	NavMeshNode(const glm::vec3& pos, const int& _gridX, const int& _gridY);
	~NavMeshNode();

	int GetFCost() const;
	glm::vec3 GetPosition() const;

	int gridX;
	int gridY;

	int gCost = 0;
	int hCost = 0;
	NavMeshNode* parent = nullptr;

	bool isColliding = false;
	glm::vec3 position;

private:
};