#pragma once

#include "Component.h"
#include "../Bounds.h"
#include "../Serialization/Serialization.h"

class Physics;

class BoxColliderComponent : public Component
{
	friend Physics;
	friend Serializer;

public:
	BoxColliderComponent(GameObject* _parent);
	~BoxColliderComponent();
	BoxColliderComponent(const BoxColliderComponent& comp);
	BoxColliderComponent& operator=(const BoxColliderComponent& comp) = delete;
	bool operator==(const BoxColliderComponent& other) const;

	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;
	
	void ShowOnInspector() override;
	void ShowOnGizmos() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

	glm::vec3 GetGlobalPosition() const;
	glm::vec3 GetLocalPosition() const;
	glm::vec3 GetCenter() const;
	glm::vec3 GetExtents() const;
	glm::vec3 GetExtentsHalf() const;

	bool IsTrigger() const;
	bool IsIgnoringNavigation() const;

	void SetExtents(const glm::vec3& newExtents);

private:
	void OnTrigger();

	bool isDynamicObstacle = false;
	bool ignoreNavigation = false;
	bool isPushable = false;
	bool isTrigger = false;
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	Bounds bounds;
};