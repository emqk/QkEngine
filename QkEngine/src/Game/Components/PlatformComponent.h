#pragma once
#include "../../Engine/Components/Component.h"
#include <glm\ext\vector_float3.hpp>
#include <array>

class PlatformComponent : public Component
{
public:
	PlatformComponent(GameObject* _parent);
	~PlatformComponent();
	
	PlatformComponent(const PlatformComponent& comp);
	PlatformComponent& operator=(const PlatformComponent& comp) = delete;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;


	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;
	virtual void ShowOnGizmos() override;

private:
	//Exposed
	std::array<glm::vec3, 2> waypoints;
	float moveSpeed = 5;

	//Not exposed
	int currWaypoint = 0;
};
