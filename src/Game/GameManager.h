#pragma once

#include <array>
#include "../GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	
private:
	std::array<GameObject*, 100> buildAreas;
	std::array<glm::vec3, 10> buildAreasPos;
};