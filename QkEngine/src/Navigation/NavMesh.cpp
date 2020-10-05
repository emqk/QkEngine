#include "NavMesh.h"
#include "../Gizmos.h"
#include "../Physics.h"
#include "../imgui/imgui.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <execution>

std::vector<NavMeshNode> NavMesh::nodes;
glm::vec3 NavMesh::startPos;
int NavMesh::width;
float NavMesh::nodeSize;
bool NavMesh::showNavMesh = true;

void NavMesh::Generate(const glm::vec3& _startPos, const int& _width, const float& _nodeSize)
{
	std::chrono::steady_clock::time_point currentSampleStartTime = std::chrono::steady_clock::now();

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

	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	float sampleDuration = dur.count();

	std::cout << "NavMesh generated in: " << sampleDuration << "ms!\n";
}

void NavMesh::CheckCollisions()
{
	std::for_each(std::execution::par, nodes.begin(), nodes.end(),
	[](NavMeshNode& currNode)
	{
		currNode.isColliding = Physics::BoxCastCheck(currNode.GetPosition(), glm::vec3(nodeSize, nodeSize, nodeSize) / 2.0f);
	});
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
	ImGui::Checkbox("Show navMesh", &showNavMesh);

	if (ImGui::Button("Generate"))
	{
		Generate(position, w, size);
	}

	ImGui::End();
}

void NavMesh::DebugDraw()
{
	if (!showNavMesh)
		return;

	for (const NavMeshNode& node : nodes)
	{
		glm::vec4 targetColor = node.isColliding ? glm::vec4(1.0f, 0.1f, 0.1f, 1) : glm::vec4(0.1f, 0.1f, 1, 1);
		Gizmos::SetCurrentColor(targetColor);
		Gizmos::DrawCubeWireframe(node.GetPosition(), glm::vec3(0, 0, 0), glm::vec3(nodeSize, nodeSize, nodeSize));
	}
}
