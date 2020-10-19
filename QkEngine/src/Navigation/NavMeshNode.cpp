#include "NavMeshNode.h"

NavMeshNode::NavMeshNode(const glm::vec3& pos, const int& _gridX, const int& _gridY) : position(pos), gridX(_gridX), gridY(_gridY), isColliding(false)
{
}

NavMeshNode::~NavMeshNode()
{
}

int NavMeshNode::GetFCost() const
{
	return gCost + hCost;;
}

glm::vec3 NavMeshNode::GetPosition() const
{
	return position;
}
