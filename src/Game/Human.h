#pragma once
#include "../GameObject.h"
#include <array>

class Human : public GameObject
{
public:
	Human(const char* meshPath, const char* shaderPath);
	~Human();

	void Update(const float& deltaTime) override;

	void GoToPoint(const glm::vec3& point, const float& deltaTime);

private:
	//Animation
	std::array<Texture*, 4> runAnim;
	int currAnimFrame = 0;
	float animTimer = 0;
	const float timeToChangeFrame = 0.25f;

	//Other
	float movementSpeed = 1.0f;
};