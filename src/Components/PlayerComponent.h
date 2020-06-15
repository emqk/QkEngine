#pragma once
#include "Component.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "MoveComponent.h"

class PlayerComponent : public Component
{
public:
	PlayerComponent(GameObject* _parent);
	~PlayerComponent();

	PlayerComponent(const PlayerComponent& comp);
	PlayerComponent& operator=(const PlayerComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	float moveSpeed = 5;
	const float minGravity = -9.81f;
	float currentGravity = 0;

	SpriteComponent* spriteComponent = nullptr;
	BoxColliderComponent* boxColliderComponent = nullptr;
	MoveComponent* moveComponent = nullptr;
};