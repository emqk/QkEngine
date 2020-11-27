#include "MoveComponent.h"

#include "../GameObject.h"

MoveComponent::MoveComponent(GameObject* _parent) : Component(_parent), moveVec(glm::vec3(0, 0, 0))
{
	name = "MoveComponent";
}

MoveComponent::~MoveComponent()
{
}

MoveComponent::MoveComponent(const MoveComponent& comp) : Component(comp)
{
	moveVec = comp.moveVec;
}

void MoveComponent::Update(const float& deltaTime)
{
	parent->Move(moveVec * deltaTime);
}

void MoveComponent::LateUpdate(const float& deltaTime)
{
}

void MoveComponent::ShowOnInspector()
{
	ImGui::InputFloat3("Move speed", &moveVec.x, 3);	
}

std::unique_ptr<Component> MoveComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<MoveComponent> comp = std::make_unique<MoveComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}