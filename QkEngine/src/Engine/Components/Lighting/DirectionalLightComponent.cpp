#include "DirectionalLightComponent.h"
#include "../../Lighting.h"
#include "../../GameObject.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* _parent) : Component(_parent)
{
	name = "DirectionalLightComponent";
	Lighting::RegisterDirectionalLight(this);
}

DirectionalLightComponent::~DirectionalLightComponent()
{
	Lighting::UnRegisterDirectionalLight(this);
}

DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponent& comp) : Component(comp)
{
	lightColor = comp.lightColor;
	Lighting::RegisterDirectionalLight(this);
}

void DirectionalLightComponent::Update(const float& deltaTime)
{
}

void DirectionalLightComponent::LateUpdate(const float& deltaTime)
{
}

void DirectionalLightComponent::ShowOnInspector()
{
	ImGui::ColorEdit3("light color", &lightColor.r, ImGuiColorEditFlags_NoInputs);
}

std::unique_ptr<Component> DirectionalLightComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<DirectionalLightComponent> comp = std::make_unique<DirectionalLightComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void DirectionalLightComponent::SetColor(const glm::vec3& color)
{
	lightColor = color;
}

glm::vec3 DirectionalLightComponent::GetPosition() const
{
	return parent->transform.GetGlobalPosition();
}

glm::vec3 DirectionalLightComponent::GetColor() const
{
	return lightColor;
}