#include "NavMesh.h"
#include "../Gizmos.h"
#include "../Physics.h"
#include "../imgui/imgui.h"
#include <iostream>

std::vector<NavMeshNode> NavMesh::nodes;
glm::vec3 NavMesh::startPos;
int NavMesh::width;
float NavMesh::nodeSize;

void NavMesh::Generate(const glm::vec3& _startPos, const int& _width, const float& _nodeSize)
{
	startPos = _startPos;
	width = _width;
	nodeSize = _nodeSize;

	nodes.clear();
	nodes.reserve(width * width);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			glm::vec3 position = glm::vec3(i * nodeSize, 0, j * nodeSize) + startPos;
			nodes.emplace_back(position);
		}
	}

	CheckCollisions();

	std::cout << "NavMesh generated!\n";
}

void NavMesh::CheckCollisions()
{
	for (NavMeshNode& currNode : nodes)
	{
		currNode.isColliding = Physics::BoxCast(currNode.position, glm::vec3(nodeSize, nodeSize, nodeSize) / 2.0f).size() > 0;
	}
}

void NavMesh::ShowNavMesh()
{
	ImGui::Begin("Navigation");

	static glm::vec3 position;
	static int w = 10;
	static float size = 2;

	ImGui::InputFloat3("Start location", &position.x, 3);
	ImGui::InputInt("Number of nodes in row", &w);
	ImGui::InputFloat("Node size", &size);

	if (ImGui::Button("Generate"))
	{
		Generate(position, w, size);
	}

	ImGui::End();
}

void NavMesh::DebugDraw()
{
	for (const NavMeshNode& node : nodes)
	{
		glm::vec4 targetColor = node.isColliding ? glm::vec4(1.0f, 0.1f, 0.1f, 1) : glm::vec4(0.1f, 0.1f, 1, 1);
		Gizmos::SetCurrentColor(targetColor);
		Gizmos::DrawCubeWireframe(node.position, glm::vec3(0, 0, 0), glm::vec3(nodeSize, nodeSize, nodeSize));
	}
}
