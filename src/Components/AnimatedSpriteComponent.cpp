#include "AnimatedSpriteComponent.h"
#include "../ResourceManager.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(GameObject* _parent) : StaticMeshComponent(_parent)
{
	name = "AnimatedSpriteComponent";
}

AnimatedSpriteComponent::~AnimatedSpriteComponent()
{
}

AnimatedSpriteComponent::AnimatedSpriteComponent(const AnimatedSpriteComponent& comp)
	: StaticMeshComponent(comp), currentAnimation(comp.currentAnimation)
{
}

void AnimatedSpriteComponent::SetCurrentAnimation(SpriteAnimation* animation)
{
	currentAnimation = animation;
}

void AnimatedSpriteComponent::Update(const float& deltaTime)
{
	if (currentAnimation != nullptr)
	{
		Texture* animationTex = currentAnimation->Update(deltaTime);
		SetTexture(animationTex);
	}
}

void AnimatedSpriteComponent::LateUpdate(const float& deltaTime)
{
}

void AnimatedSpriteComponent::ShowOnInspector()
{
	StaticMeshComponent::ShowOnInspector();
}

std::unique_ptr<Component> AnimatedSpriteComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<AnimatedSpriteComponent> comp = std::make_unique<AnimatedSpriteComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}
