#include "PointLightComponent.h"
#include "../../Lighting.h"
#include "../../GameObject.h"

PointLightComponent::PointLightComponent(GameObject* _parent) : Component(_parent)
{
	name = "PointLightComponent";
	Lighting::RegisterPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
	Lighting::UnRegisterPointLight(this);
}

PointLightComponent::PointLightComponent(const PointLightComponent& comp) : Component(comp), lightColor(comp.lightColor), intensity(comp.intensity)
{
	Lighting::RegisterPointLight(this);
}

void PointLightComponent::Update(const float& deltaTime)
{
}

void PointLightComponent::LateUpdate(const float& deltaTime)
{
}

void PointLightComponent::ShowOnInspector()
{
	ImGui::ColorEdit3("light color", &lightColor.r, ImGuiColorEditFlags_NoInputs);
	ImGui::DragFloat("Intensity", &intensity, 0.1f);
}

std::unique_ptr<Component> PointLightComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<PointLightComponent> comp = std::make_unique<PointLightComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

glm::vec3 PointLightComponent::GetPosition() const
{
	return parent->transform.GetGlobalPosition();
}

glm::vec3 PointLightComponent::GetColor() const
{
	return lightColor;
}

float PointLightComponent::GetIntensity() const
{
	return intensity;
}