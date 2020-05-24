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
	void ShowOnInspector() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

	glm::vec3 GetExtents() const;

private:
	Bounds bounds;
};