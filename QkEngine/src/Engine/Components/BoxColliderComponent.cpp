#include "BoxColliderComponent.h"
#include "StaticMeshComponent.h"
#include "../GameObject.h"
#include "../Physics/Physics.h"
#include "../Navigation/NavMesh.h"
#include "../Gizmos.h"

BoxColliderComponent::BoxColliderComponent(GameObject* _parent) : Component(_parent)
{
	name = "BoxColliderComponent";
	StaticMeshComponent* staticMeshComponent = parent->GetComponent<StaticMeshComponent>();
	if (staticMeshComponent != nullptr)
	{
		if (staticMeshComponent->GetMesh() != nullptr)
			SetExtents(staticMeshComponent->GetMesh()->GetBounds().Extents());
		else
			SetExtents(glm::vec3(1, 1, 1));
	}
	else
	{
		SetExtents(glm::vec3(1, 1, 1));
	}

	Physics::RegisterCollider(this);
}

BoxColliderComponent::~BoxColliderComponent()
{
	Physics::UnRegisterCollider(this);
}

BoxColliderComponent::BoxColliderComponent(const BoxColliderComponent& comp)
	: Component(comp), bounds(comp.bounds), center(comp.center), isTrigger(comp.isTrigger), isPushable(comp.isPushable)
	, isDynamicObstacle(comp.isDynamicObstacle), ignoreNavigation(comp.ignoreNavigation)
{
	Physics::RegisterCollider(this);
}

bool BoxColliderComponent::operator==(const BoxColliderComponent& other) const
{
	return this == &other;
}

void BoxColliderComponent::Update(const float& deltaTime)
{
}

void BoxColliderComponent::LateUpdate(const float& deltaTime)
{
	if (isDynamicObstacle)
	{
		glm::vec3 topLeft = parent->transform.GetGlobalPosition() + center - GetExtentsHalf();
		glm::vec3 bottomRight = parent->transform.GetGlobalPosition() + center + GetExtentsHalf();
		NavMesh::RegenerateChunk(topLeft, bottomRight);
	}
}

void BoxColliderComponent::ShowOnInspector()
{
	//Is Dynamic navMesh obstacle
	ImGui::Checkbox("Is Dynamic Obstacle", &isDynamicObstacle);

	//Should ignore navigation?
	ImGui::Checkbox("Ignore Navigation", &ignoreNavigation);

	//Pushable
	ImGui::Checkbox("Is Pushable", &isPushable);

	//Trigger
	ImGui::Checkbox("Is Trigger", &isTrigger);

	//Bounds
	glm::vec3 boundsExtents = bounds.Extents();
	ImGui::DragFloat3("Extents", &boundsExtents.x, 0.1f);
	bounds.SetExtents(boundsExtents);

	//Position offset
	ImGui::DragFloat3("Center", &center.x, 0.1f);
}

void BoxColliderComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(Gizmos::collisionColor);
	Gizmos::DrawCubeWireframe(GetGlobalPosition(), glm::vec3(0,0,0), bounds.Extents());
}

std::unique_ptr<Component> BoxColliderComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<BoxColliderComponent> comp = std::make_unique<BoxColliderComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

glm::vec3 BoxColliderComponent::GetGlobalPosition() const
{
	return parent->transform.GetGlobalPosition() + center;
}

glm::vec3 BoxColliderComponent::GetLocalPosition() const
{
	return parent->transform.GetLocalPosition() + center;
}

glm::vec3 BoxColliderComponent::GetCenter() const
{
	return center;
}

glm::vec3 BoxColliderComponent::GetExtents() const
{
	return bounds.Extents();
}

glm::vec3 BoxColliderComponent::GetExtentsHalf() const
{
	return bounds.ExtentsHalf();
}

bool BoxColliderComponent::IsTrigger() const
{
	return isTrigger;
}

bool BoxColliderComponent::IsIgnoringNavigation() const
{
	return ignoreNavigation;
}

void BoxColliderComponent::SetExtents(const glm::vec3& newExtents)
{
	bounds.SetExtents(newExtents);
}

void BoxColliderComponent::OnTrigger()
{
	std::cout << "In trigger!\n";
}