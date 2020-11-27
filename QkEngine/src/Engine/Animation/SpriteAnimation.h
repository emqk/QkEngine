#pragma once

#include <vector>

#include "../Components/StaticMeshComponent.h"
#include "../Texture.h"

class SpriteAnimation
{
	friend class Serializer;

public:
	SpriteAnimation(std::vector<Texture*> _textures);
	~SpriteAnimation();

	SpriteAnimation(const SpriteAnimation& other);
	SpriteAnimation& operator=(const SpriteAnimation& other);
	SpriteAnimation (SpriteAnimation&& other) noexcept;
	SpriteAnimation& operator=(SpriteAnimation&& other) noexcept;

	size_t GetFramesCount();
	Texture* GetTexture(const size_t index);

	std::string name;

private:
	std::vector<Texture*> textures;
};

