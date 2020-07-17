#pragma once
#include "Component.h"
#include <glm\ext\vector_float3.hpp>

class LightComponent : public Component
{
public:
	LightComponent(GameObject* _parent);
	~LightComponent();

	LightComponent(const LightComponent& comp);
	LightComponent& operator=(const LightComponent& comp) = delete;

	virtual void Update(const float& deltaTime) override;
	virtual void LateUpdate(const float& deltaTime) override;

	virtual void ShowOnInspector() override;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;


	glm::vec3 GetColor() const;

private:
	glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
};