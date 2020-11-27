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
	: StaticMeshComponent(comp), currentAnimation(comp.currentAnimation), frameTime(comp.frameTime), currFrame(comp.currFrame), currTime(comp.currTime)
{
}

void AnimatedSpriteComponent::SetCurrentAnimation(SpriteAnimation* animation)
{
	if (currentAnimation != animation)
	{
		currentAnimation = animation;
		currFrame = 0;
	}
}

void AnimatedSpriteComponent::Update(const float& deltaTime)
{
	if (currentAnimation != nullptr)
	{
		currTime += deltaTime;
		if (currTime > frameTime)
		{
			currFrame++;
			if (currFrame >= currentAnimation->GetFramesCount())
				currFrame = 0;

			currTime = 0;
		}
		SetTexture(currentAnimation->GetTexture(currFrame));
	}
}

void AnimatedSpriteComponent::LateUpdate(const float& deltaTime)
{
}

void AnimatedSpriteComponent::ShowOnInspector()
{
	ImGui::DragFloat("Frame time", &frameTime, 0.025f);
	if (frameTime < 0)
		frameTime = 0;

	StaticMeshComponent::ShowOnInspector();
}

std::unique_ptr<Component> AnimatedSpriteComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<AnimatedSpriteComponent> comp = std::make_unique<AnimatedSpriteComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}