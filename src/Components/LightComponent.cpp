#include "LightComponent.h"
#include "../Lighting.h"

LightComponent::LightComponent(GameObject* _parent) : Component(_parent)
{
	name = "LightComponent";
	Lighting::RegisterLight(this);
}

LightComponent::~LightComponent()
{
	Lighting::UnRegisterLight(this);
}

LightComponent::LightComponent(const LightComponent& comp) : Component(comp)
{
	lightColor = comp.lightColor;
	Lighting::RegisterLight(this);
}

void LightComponent::Update(const float& deltaTime)
{
}

void LightComponent::LateUpdate(const float& deltaTime)
{
}

void LightComponent::ShowOnInspector()
{
	ImGui::ColorEdit3("Fog color", &lightColor.r, ImGuiColorEditFlags_NoInputs);
}

std::unique_ptr<Component> LightComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<LightComponent> comp = std::make_unique<LightComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

glm::vec3 LightComponent::GetColor() const
{
	return lightColor;
}