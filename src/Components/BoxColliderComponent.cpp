#include "BoxColliderComponent.h"
#include "SpriteComponent.h"
#include "../GameObject.h"
#include "../Physics.h"
#include "../Gizmos.h"

#include <iostream>

BoxColliderComponent::BoxColliderComponent(GameObject* _parent) : Component(_parent)
{
	name = "BoxColliderComponent";
	SpriteComponent* spriteComponent = parent->GetComponent<SpriteComponent>();
	if (spriteComponent != nullptr)
	{
		bounds = spriteComponent->GetMesh()->GetBounds();
	}
	else
	{
		bounds.SetExtents(glm::vec3(1, 1, 1));
	}

	Physics::RegisterCollider(this);
}

BoxColliderComponent::~BoxColliderComponent()
{
	Physics::UnRegisterCollider(this);
	std::cout << "Box Collider Component destructor\n";
}

BoxColliderComponent::BoxColliderComponent(const BoxColliderComponent& comp) : Component(comp), bounds(comp.bounds), center(comp.center)
{
	std::cout << "BoxColliderComp copy constuctor\n";
	Physics::RegisterCollider(this);
}

bool BoxColliderComponent::operator==(const BoxColliderComponent& other) const
{
	return this == &other;
}

void BoxColliderComponent::Update(const float& deltaTime)
{
}

void BoxColliderComponent::ShowOnInspector()
{
	ImGui::Text("Properties");

	//Bounds
	glm::vec3 boundsExtents = bounds.Extents();
	ImGui::InputFloat3("Extents", &boundsExtents.x, 3);
	bounds.SetExtents(boundsExtents);

	//Position offset
	ImGui::InputFloat3("Center", &center.x, 3);
}

void BoxColliderComponent::ShowOnGizmos()
{
	Gizmos::DrawCubeWireframe(GetPosition(), parent->GetRotation(), bounds.Extents());
}

std::unique_ptr<Component> BoxColliderComponent::MakeCopy(GameObject* newParent) const
{
	std::cout << "Get copy of moveComp\n";
	std::unique_ptr<BoxColliderComponent> comp = std::make_unique<BoxColliderComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

glm::vec3 BoxColliderComponent::GetPosition() const
{
	return parent->GetPosition() + center;
}

glm::vec3 BoxColliderComponent::GetCenter() const
{
	return center;
}

glm::vec3 BoxColliderComponent::GetExtents() const
{
	return bounds.Extents();
}