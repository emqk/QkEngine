#pragma once

#include "StaticMeshComponent.h"
#include "../Animation/SpriteAnimation.h"

class AnimatedSpriteComponent : public StaticMeshComponent
{
public:
	AnimatedSpriteComponent(GameObject* _parent);
	~AnimatedSpriteComponent();

	AnimatedSpriteComponent(const AnimatedSpriteComponent& comp);
	AnimatedSpriteComponent& operator=(const AnimatedSpriteComponent& comp) = delete;

	void SetCurrentAnimation(SpriteAnimation* animation);

	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

	void ShowOnInspector() override;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	SpriteAnimation* currentAnimation = nullptr;
};