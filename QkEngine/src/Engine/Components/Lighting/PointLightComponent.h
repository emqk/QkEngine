#pragma once
#include "../Component.h"
#include <glm\ext\vector_float3.hpp>

class PointLightComponent : public Component
{
	friend class Serializer;

public:
	PointLightComponent(GameObject* _parent);
	~PointLightComponent();

	PointLightComponent(const PointLightComponent& comp);
	PointLightComponent& operator=(const PointLightComponent& comp) = delete;

	virtual void Update(const float& deltaTime) override;
	virtual void LateUpdate(const float& deltaTime) override;

	virtual void ShowOnInspector() override;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

	glm::vec3 GetPosition() const;
	glm::vec3 GetColor() const;
	float GetIntensity() const;

private:
	glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
	float intensity = 1;
};