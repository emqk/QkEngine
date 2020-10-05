#pragma once
#include <glm\ext\vector_float3.hpp>

class NavMeshNode
{
public:
	NavMeshNode(const glm::vec3& pos);
	~NavMeshNode();

	glm::vec3 position;
	bool isColliding = false;

private:

};