#pragma once

#include "Component.h"
#include "../Bounds.h"

class BoxColliderComponent : public Component
{
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

	glm::vec3 GetLocalPosition() const;
	glm::vec3 GetCenter() const;
	glm::vec3 GetExtents() const;

	void SetExtents(const glm::vec3& newExtents);

private:
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	Bounds bounds;
};