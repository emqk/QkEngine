#include "PlatformComponent.h"
#include "../../Engine/Gizmos.h"

PlatformComponent::PlatformComponent(GameObject* _parent)
	: Component(_parent)
	, waypoints{ _parent->transform.GetGlobalPosition(), _parent->transform.GetGlobalPosition() + glm::vec3(1, 0, 0) }
{
	name = "PlatformComponent";
}

PlatformComponent::~PlatformComponent()
{
}

PlatformComponent::PlatformComponent(const PlatformComponent& comp) : Component(comp), waypoints(comp.waypoints), moveSpeed(comp.moveSpeed)
{
}

std::unique_ptr<Component> PlatformComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<PlatformComponent> comp = std::make_unique<PlatformComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void PlatformComponent::Update(const float& deltaTime)
{
	if (parent->transform.MoveTo(waypoints[currWaypoint], moveSpeed * deltaTime))
	{
		currWaypoint++;

		if (currWaypoint >= waypoints.size())
			currWaypoint = 0;
	}
}

void PlatformComponent::ShowOnInspector()
{
	ImGui::DragFloat3("PointA", &waypoints[0].x, 0.1f);
	ImGui::DragFloat3("PointB", &waypoints[1].x, 0.1f);
	ImGui::DragFloat("Move Speed", &moveSpeed, 0.1f);
}

void PlatformComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(glm::vec4(0.75f, 0.75f, 1.0f, 1.0f));
	Gizmos::DrawCubeWireframe(waypoints[0], glm::quat(0,0,0,0), glm::vec3(0.5f, 0.5f, 0.5f));
	Gizmos::SetCurrentColor(glm::vec4(0.75f, 0.75f, 1.0f, 1.0f));
	Gizmos::DrawCubeWireframe(waypoints[1], glm::quat(0,0,0,0), glm::vec3(0.5f, 0.5f, 0.5f));
}
