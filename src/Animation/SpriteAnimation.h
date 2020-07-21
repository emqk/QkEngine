#pragma once

#include <vector>

#include "../Components/StaticMeshComponent.h"
#include "../Texture.h"

class SpriteAnimation
{
public:
	SpriteAnimation(std::vector<Texture*> _textures);
	~SpriteAnimation();

	SpriteAnimation(const SpriteAnimation& other);
	SpriteAnimation& operator=(const SpriteAnimation& other);
	SpriteAnimation (SpriteAnimation&& other) noexcept;

	Texture* Update(const float& deltaTime);

private:
	std::vector<Texture*> textures;
	float timeToChangeFrame = 0.2f;
	size_t currFrame = 0;
	float currTime = 0;
};

