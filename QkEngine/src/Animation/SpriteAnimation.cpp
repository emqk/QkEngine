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
	return *this;
}

SpriteAnimation::SpriteAnimation(SpriteAnimation&& other) noexcept
{
	*this = std::move(other);
}

SpriteAnimation& SpriteAnimation::operator=(SpriteAnimation&& other) noexcept
{
	textures = std::move(other.textures);
	return *this;
}

size_t SpriteAnimation::GetFramesCount()
{
	return textures.size();
}

Texture* SpriteAnimation::GetTexture(const size_t index)
{
	return textures[index];
}