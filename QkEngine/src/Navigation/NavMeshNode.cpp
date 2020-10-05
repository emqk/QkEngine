#include "NavMeshNode.h"

NavMeshNode::NavMeshNode(const glm::vec3& pos) : position(pos), isColliding(false)
{
}

NavMeshNode::~NavMeshNode()
{
}