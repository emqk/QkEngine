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
	nodes.reserve(width);
	for (int y = 0; y < width; y++)
	{
		nodes.reserve(width);
		for (int x = 0; x < width; x++)
		{
			glm::vec3 position = glm::vec3(x * nodeSize, 0, y * nodeSize) + startPos;
			nodes.emplace_back(position, x, y);
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

std::vector<NavMeshNode*> NavMesh::GetPathTest()
{
	std::chrono::steady_clock::time_point currentSampleStartTime = std::chrono::steady_clock::now();

	std::vector<NavMeshNode*> path = GetPath(GetNodeAt2DIndex(0, 0), GetNodeAt2DIndex(9, 9));

	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	float sampleDuration = dur.count();
	std::cout << "Path found in: " << sampleDuration << "ms. Path size: " << path.size() << "\n";

	return path;
}

std::vector<NavMeshNode*> NavMesh::GetPath(NavMeshNode* startNode, NavMeshNode* endNode)
{
	std::vector<NavMeshNode*> openSet;
	std::vector<NavMeshNode*> closedSet;
	openSet.push_back(startNode);

	while (openSet.size() > 0)
	{
		NavMeshNode* currNode = openSet[0];
		for (NavMeshNode* openSetNode : openSet)
		{
			if (openSetNode->GetFCost() <= currNode->GetFCost())
			{
				if (openSetNode->hCost < currNode->hCost)
					currNode = openSetNode;
			}
		}

		auto toEraseIt = std::find(openSet.begin(), openSet.end(), currNode);
		openSet.erase(toEraseIt);
		closedSet.push_back(currNode);

		if (currNode == endNode)
		{
			return RetracePath(startNode, endNode);
		}

		for (NavMeshNode* neighbour : GetNeighbours(currNode))
		{
			auto neighbourInClosedSetIt = std::find(closedSet.begin(), closedSet.end(), neighbour);
			if (neighbour->isColliding || neighbourInClosedSetIt != closedSet.end())
			{
				continue;
			}

			int newCostToNeighbour = currNode->gCost + GetDistance(currNode, neighbour);
			auto neighbourInOpenSetIt = std::find(openSet.begin(), openSet.end(), neighbour);
			if (newCostToNeighbour < neighbour->gCost || neighbourInOpenSetIt == openSet.end())
			{
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, endNode);
				neighbour->parent = currNode;

				if (neighbourInOpenSetIt == openSet.end())
				{
					openSet.push_back(neighbour);
				}
			}
		}
	}

	return std::vector<NavMeshNode*>();
}

void NavMesh::ShowNavMesh()
{
	ImGui::Begin("Navigation");

	//Default values
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

NavMeshNode* NavMesh::GetNodeAt2DIndex(const int& x, const int& y)
{
	if (x >= 0 && x < width && y >= 0 && y < width)
	{
		return &nodes[width * y + x];
	}

	return nullptr;
}

std::vector<NavMeshNode*> NavMesh::GetNeighbours(const NavMeshNode const* node)
{
	std::vector<NavMeshNode*> result;
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (x == 0 && y == 0)
				continue;

			int checkX = node->gridX + x;
			int checkY = node->gridY + y;

			if (checkX >= 0 && checkX < width && checkY >= 0 && checkY < width)
			{
				//std::cout << "getting node " << checkX << "x " << checkY << "y" << "\n";
				result.push_back(GetNodeAt2DIndex(checkX, checkY));
			}
		}
	}

	return std::move(result);
}

std::vector<NavMeshNode*> NavMesh::RetracePath(NavMeshNode* startNode, NavMeshNode* endNode)
{
	std::vector<NavMeshNode*> path;
	NavMeshNode* currNode = endNode;

	while (currNode != startNode)
	{
		path.push_back(currNode);
		currNode = currNode->parent;
	}

	std::cout << "Retraced Path size: " << path.size() << "\n";

	return std::move(path);
}

int NavMesh::GetDistance(const NavMeshNode* nodeA, const NavMeshNode* nodeB)
{
	int dstX = abs(nodeA->gridX - nodeB->gridX);
	int dstY = abs(nodeA->gridY - nodeB->gridY);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}
