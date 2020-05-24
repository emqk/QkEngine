#include "Human.h"
#include "../ResourceManager.h"

Human::Human(const char* meshPath, const char* shaderPath) : GameObject()
{
	//SetTexture(ResourceManager::GetTexture("Human/Human_Walk_1.png"));

	//runAnim[0] = ResourceManager::GetTexture("Human/Human_Walk_1.png");
	//runAnim[1] = ResourceManager::GetTexture("Human/Human_Walk_2.png");
	//runAnim[2] = ResourceManager::GetTexture("Human/Human_Walk_3.png");
	//runAnim[3] = ResourceManager::GetTexture("Human/Human_Walk_4.png");
}

Human::~Human()
{
}

void Human::Update(const float& deltaTime)
{
	animTimer += deltaTime;
	GoToPoint(glm::vec3(-5, 0, 0), deltaTime);

	if (animTimer >= timeToChangeFrame)
	{
		animTimer = 0;
		if (currAnimFrame + 1 > runAnim.size() - 1)
			currAnimFrame = 0;
		else
			currAnimFrame++;

		//SetTexture(runAnim[currAnimFrame]);
	}
}

void Human::GoToPoint(const glm::vec3& point, const float& deltaTime)
{
	glm::vec3 myPos = GetPosition();

	float distX = abs(myPos.x - point.x);
	if (distX <= movementSpeed * deltaTime)
	{
		SetPosition(glm::vec3(point.x, myPos.y, myPos.z));
		return;
	}

	glm::vec3 myScale = GetScale();
	if (point.x > GetPosition().x)
	{
		Move(glm::vec3(movementSpeed, 0.0f, 0.0f) * deltaTime);
		SetScale(glm::vec3(-abs(myScale.x), myScale.y, myScale.z));
	}
	else
	{
		Move(glm::vec3(-movementSpeed, 0.0f, 0.0f) * deltaTime);
		SetScale(glm::vec3(abs(myScale.x), myScale.y, myScale.z));
	}
}
