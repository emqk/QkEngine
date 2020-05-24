#pragma once
#include "Component.h"
#include <glm\ext\vector_float3.hpp>

class MoveComponent : public Component
{
public:
	MoveComponent(GameObject* _parent);
	~MoveComponent();

	MoveComponent(const MoveComponent& comp);
	MoveComponent& operator=(const MoveComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

protected:

private:
	glm::vec3 moveVec;

};