#pragma once
#include "../../Engine/Components/Component.h"
#include "../../Engine/Components/AnimatedSpriteComponent.h"
#include "../../Engine/Components/BoxColliderComponent.h"
#include "../../Engine/Components/MoveComponent.h"
#include "../../Engine/UI/ButtonWidget.h"

class PlayerComponent : public Component
{
	friend Serializer;

public:
	PlayerComponent(GameObject* _parent);
	~PlayerComponent();

	PlayerComponent(const PlayerComponent& comp);
	PlayerComponent& operator=(const PlayerComponent& comp) = delete;

	void Start() override;
	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

	void ShowOnInspector() override;
	void ShowOnGizmos() override;
	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

protected:
	void ControlAnimations(const glm::vec3& moveVec);

private:
	float moveSpeed = 5;
	const float minGravity = -9.81f;
	float currentGravity = 0;
	glm::vec3 groundDetectorOffset{ 0, -1, 0 };
	glm::vec3 groundDetectorScale{ 0.5f, 0.1f, 0.5f };
	
	bool isGrounded = false;

	Widget* playerWidget = nullptr;
	ButtonWidget* playerButtonWidget = nullptr;

	BoxColliderComponent* boxColliderComponent = nullptr;
	MoveComponent* moveComponent = nullptr;
	AnimatedSpriteComponent* animatedSpriteComponent = nullptr;
};