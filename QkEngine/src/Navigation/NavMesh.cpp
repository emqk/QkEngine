#include "NavMesh.h"
#include "../Gizmos.h"
#include "../imgui/imgui.h"
#include <iostream>

std::vector<glm::vec3> NavMesh::nodesLocation;
glm::vec3 NavMesh::startPos;
int NavMesh::width;
float NavMesh::nodeSize;

void NavMesh::Generate(const glm::vec3& _startPos, const int& _width, const float& _nodeSize)
{
	startPos = _startPos;
	width = _width;
	nodeSize = _nodeSize;

	nodesLocation.clear();
	nodesLocation.reserve(width * width);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			nodesLocation.push_back((glm::vec3(i * nodeSize, 0, j * nodeSize)) + startPos);
		}
	}

	std::cout << "NavMesh generated!\n";
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
	Gizmos::SetCurrentColor(glm::vec4(0, 0, 1, 1));
	for (const glm::vec3& loc : nodesLocation)
	{
		Gizmos::DrawCubeWireframe(loc, glm::vec3(0, 0, 0), glm::vec3(nodeSize, nodeSize, nodeSize));
	}
}
