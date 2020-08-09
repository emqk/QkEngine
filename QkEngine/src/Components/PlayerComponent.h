#pragma once
#include "Component.h"
#include "AnimatedSpriteComponent.h"
#include "BoxColliderComponent.h"
#include "MoveComponent.h"
#include <array>

class PlayerComponent : public Component
{
public:
	PlayerComponent(GameObject* _parent);
	~PlayerComponent();

	PlayerComponent(const PlayerComponent& comp);
	PlayerComponent& operator=(const PlayerComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

	void ShowOnInspector() override;
	void ShowOnGizmos() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	float moveSpeed = 5;
	const float minGravity = -9.81f;
	float currentGravity = 0;
	glm::vec3 groundDetectorOffset{ 0, -1, 0 };
	glm::vec3 groundDetectorScale{ 0.5f, 0.1f, 0.5f };

	BoxColliderComponent* boxColliderComponent = nullptr;
	MoveComponent* moveComponent = nullptr;
	AnimatedSpriteComponent* animatedSpriteComponent = nullptr;
};