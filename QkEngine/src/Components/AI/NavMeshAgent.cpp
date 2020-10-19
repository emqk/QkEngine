#include "NavMeshAgent.h"
#include "../../GameObject.h"
#include "../../Navigation/NavMesh.h"

NavMeshAgentComponent::NavMeshAgentComponent(GameObject* _parent) : Component(_parent)
{
	name = "NavMeshAgentComponent";
}

NavMeshAgentComponent::~NavMeshAgentComponent()
{
}

NavMeshAgentComponent::NavMeshAgentComponent(const NavMeshAgentComponent& comp) : Component(comp), movementSpeed(comp.movementSpeed), path(comp.path)
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
	if (path.size() > 0)
	{
		MoveTo(path[0], deltaTime);
	}
	else
	{
		std::vector<glm::vec3> pos;
		for (auto n : NavMesh::GetPathTest())
		{
			pos.push_back(n->position);
		}
		SetPath(pos);
	}
}

void NavMeshAgentComponent::ShowOnInspector()
{
	ImGui::DragFloat("Movement speed", &movementSpeed, 0.1f);
}

void NavMeshAgentComponent::SetPath(const std::vector<glm::vec3>& newPath)
{
	path = newPath;
}

bool NavMeshAgentComponent::MoveTo(const glm::vec3& position, const float& deltaTime)
{
	float frameDistance = movementSpeed * deltaTime;
	float distToTarget = glm::length(position - parent->transform.GetGlobalPosition());
	if (distToTarget <= frameDistance)
	{
		parent->transform.SetGlobalPosition(position);
		return true;
	}
	else
	{
		glm::vec3 dirToTarget = glm::normalize(position - parent->transform.GetGlobalPosition());
		parent->transform.Translate(dirToTarget * frameDistance);
	}

	return false;
}
