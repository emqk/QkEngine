#pragma once

#include <array>
#include "../Engine/GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	GameObject* playerObj;

private:
	std::array<GameObject*, 10> buildAreas;
	std::array<glm::vec3, 10> buildAreasPos;
};