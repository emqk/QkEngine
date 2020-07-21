#include "SpriteAnimation.h"
#include "../ResourceManager.h"

SpriteAnimation::SpriteAnimation(std::vector<Texture*> _textures)
{
	textures = _textures;
}

SpriteAnimation::~SpriteAnimation()
{
}

SpriteAnimation::SpriteAnimation(const SpriteAnimation& other)
{
	*this = other;
}

SpriteAnimation& SpriteAnimation::operator=(const SpriteAnimation& other)
{
	textures = other.textures;
	timeToChangeFrame = other.timeToChangeFrame;
	currFrame = other.currFrame;
	currTime = other.currTime;

	return *this;
}

SpriteAnimation::SpriteAnimation(SpriteAnimation&& other) noexcept
	: textures(std::move(other.textures)), timeToChangeFrame(other.timeToChangeFrame), currFrame(other.currFrame), currTime(other.currTime)
{

}

Texture* SpriteAnimation::Update(const float& deltaTime)
{
	currTime += deltaTime;
	if (currTime > timeToChangeFrame)
	{
		currFrame++;
		if (currFrame >= textures.size())
			currFrame = 0;

		currTime = 0;
	}

	return textures[currFrame];
}