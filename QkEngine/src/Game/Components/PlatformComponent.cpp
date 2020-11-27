#include "PlatformComponent.h"

PlatformComponent::PlatformComponent(GameObject* _parent) : Component(_parent)
{
	name = "PlatformComponent";
}

PlatformComponent::~PlatformComponent()
{
}

PlatformComponent::PlatformComponent(const PlatformComponent& comp) : Component(comp)
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
}

void PlatformComponent::ShowOnInspector()
{
	ImGui::LabelText("To do..", "Things...");
}
