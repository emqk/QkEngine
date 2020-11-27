#include "NavMeshAgent.h"
#include "../../GameObject.h"
#include "../../Navigation/NavMesh.h"
#include "../../Scene.h"
#include "../PlayerComponent.h"

NavMeshAgentComponent::NavMeshAgentComponent(GameObject* _parent) : Component(_parent)
{
	name = "NavMeshAgentComponent";
}

NavMeshAgentComponent::~NavMeshAgentComponent()
{
}

NavMeshAgentComponent::NavMeshAgentComponent(const NavMeshAgentComponent& comp) : Component(comp), movementSpeed(comp.movementSpeed), path(comp.path), currentPathIndex(comp.currentPathIndex)
{
}

std::unique_ptr<Component> NavMeshAgentComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<NavMeshAgentComponent> comp = std::make_unique<NavMeshAgentComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void NavMeshAgentComponent::Update(const float& deltaTime)
{
	GameObject* player = Scene::GetCurrentScene().FindObjectWithComponent<PlayerComponent>();
	SetPath(NavMesh::GetPath(parent->transform.GetGlobalPosition(), player->transform.GetGlobalPosition()));
	if (path.size() > 0)
	{
		FollowPath(deltaTime);
	}
}

void NavMeshAgentComponent::ShowOnInspector()
{
	ImGui::DragFloat("Movement speed", &movementSpeed, 0.1f);
}

void NavMeshAgentComponent::SetPath(const std::vector<NavMeshNode*>& newPath)
{
	path = newPath;
	currentPathIndex = 0;
}

bool NavMeshAgentComponent::FollowPath(const float& deltaTime)
{
	if (currentPathIndex >= 0 && currentPathIndex < path.size())
	{
		if (parent->transform.MoveTo(path[currentPathIndex]->position, movementSpeed * deltaTime))
		{
			currentPathIndex++;
		}

		return false;
	}

	if (currentPathIndex >= path.size())
	{
		return true;
	}

	return false;
}
