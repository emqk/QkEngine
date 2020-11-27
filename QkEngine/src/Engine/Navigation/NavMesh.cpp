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
	//std::chrono::steady_clock::time_point currentSampleStartTime = std::chrono::steady_clock::now();

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

	//std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	//std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	//float sampleDuration = dur.count();

	//std::cout << "NavMesh generated in: " << sampleDuration << "ms!\n";
}

void NavMesh::RegenerateChunk(const glm::vec3& _startPos, const glm::vec3& _endPos)
{
	//std::chrono::steady_clock::time_point currentSampleStartTime = std::chrono::steady_clock::now();

	const NavMeshNode* startNode = GetNodeFromPosition(_startPos + glm::vec3(-nodeSize, 0, -nodeSize));
	const NavMeshNode* endNode = GetNodeFromPosition(_endPos + glm::vec3(nodeSize, 0, nodeSize));

	if (!startNode || !endNode)
	{
		std::cout << "NavMesh chunk can't be regenerated!: startNode == nullptr or endNode == nullptr!\n";
		return;
	}

	for (size_t y = startNode->gridY; y <= endNode->gridY; y++)
	{
		for (size_t x = startNode->gridX; x <= endNode->gridX; x++)
		{
			NavMeshNode* currNode = GetNodeAt2DIndex(x, y);
			RefreshCollisionForNode(currNode);
		}
	}

	//std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	//std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	//float sampleDuration = dur.count();
	//std::cout << "NavMesh chunk regenerated in: " << sampleDuration << "ms.\n";
}

void NavMesh::CheckCollisions()
{
	std::for_each(std::execution::par, nodes.begin(), nodes.end(),
	[](NavMeshNode& currNode)
	{
		RefreshCollisionForNode(&currNode);
	});
}

std::vector<NavMeshNode*> NavMesh::GetPath(const glm::vec3& startPos, const glm::vec3& endPos)
{
	//std::chrono::steady_clock::time_point currentSampleStartTime = std::chrono::steady_clock::now();

	std::vector<NavMeshNode*> path = GetPath(GetNodeFromPosition(startPos), GetNodeFromPosition(endPos));

	//std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	//std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	//float sampleDuration = dur.count();
	//std::cout << "Path found in: " << sampleDuration << "ms. Path size: " << path.size() << "\n";

	return path;
}

std::vector<NavMeshNode*> NavMesh::GetPath(NavMeshNode* startNode, NavMeshNode* endNode)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		std::cout << "Can't find path - startNode == nullptr or endNode == nullptr\n";
		return {};
	}

	if (endNode->isColliding)
	{
		std::cout << "Can't find path - end node is not walkable!\n";
		return {};
	}

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

	return {};
}

void NavMesh::ShowNavMesh()
{
	ImGui::Begin("Navigation");

	//Default values
	static glm::vec3 position(-20, 1, -20);
	static int w = 40;
	static float size = 1;

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

void NavMesh::RefreshCollisionForNode(NavMeshNode* node)
{
	std::vector<BoxColliderComponent*> colliders = Physics::BoxCast(node->GetPosition(), glm::vec3(nodeSize, nodeSize, nodeSize) / 2.0f);
	for (const BoxColliderComponent* coll : colliders)
	{
		if (!coll->IsIgnoringNavigation())
		{
			node->isColliding = true;
			return;
		}
	}
	node->isColliding = false;
}

NavMeshNode* NavMesh::GetNodeFromPosition(const glm::vec3& position)
{
	int x = (position.x - startPos.x) / nodeSize;
	int y = (position.z - startPos.z) / nodeSize;
	return GetNodeAt2DIndex(x, y);
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

	std::reverse(path.begin(), path.end());
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
