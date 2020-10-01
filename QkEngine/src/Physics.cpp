#include "Physics.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game/GameManager.h"
#include "Components/MoveComponent.h"
#include "Components/PlayerComponent.h"

#include <iostream>
#include <algorithm>

std::vector<BoxColliderComponent*> Physics::colliderComponents;


void Physics::Perform()
{
	for (size_t i = 0; i < colliderComponents.size(); i++)
	{
		BoxColliderComponent* currentBoxColl = colliderComponents[i];
		if (!currentBoxColl->IsActive() || currentBoxColl->GetParent()->GetComponent<MoveComponent>() == nullptr)
		{
			continue;
		}

		std::cout << "Start checking collision for: " << currentBoxColl->GetParent()->name << "\n";

		CheckCollisionFor(currentBoxColl);
	}
}

std::vector<BoxColliderComponent*> Physics::BoxCast(const glm::vec3& pos, const glm::vec3& extents)
{
	std::vector<BoxColliderComponent*> result;
	
	for (size_t j = 0; j < colliderComponents.size(); j++)
	{
		BoxColliderComponent* otherBoxColl = colliderComponents[j];
	
		if (!otherBoxColl->IsActive())
			continue;
		
		glm::vec3 otherObjectPosition = otherBoxColl->GetLocalPosition();
		glm::vec3 otherExtents = otherBoxColl->GetExtents();
	
		glm::vec3 delta = pos - otherObjectPosition;
		glm::vec3 extentsSum = extents + otherExtents;
	
		glm::vec3 intersection = glm::vec3(abs(delta.x) - extentsSum.x, abs(delta.y) - extentsSum.y, abs(delta.z) - extentsSum.z);
	
		if (intersection.x < 0 && intersection.y < 0 && intersection.z < 0)
		{
			result.push_back(otherBoxColl);
		}
	}
	
	return std::move(result);
}

void Physics::RegisterCollider(BoxColliderComponent* comp)
{
	colliderComponents.push_back(comp);
	//std::cout << "BoxColliderComponent registered\n";
}

void Physics::UnRegisterCollider(BoxColliderComponent* comp)
{
    std::vector<BoxColliderComponent*>::iterator it = std::find(colliderComponents.begin(), colliderComponents.end(), comp);
    if (it != colliderComponents.end())
    {
        colliderComponents.erase(it);
       // std::cout << "BoxColliderComponent removed from physics: " << colliderComponents.size() << "\n";
    }
}

void Physics::CheckCollisionFor(BoxColliderComponent* boxCollider)
{
	for (size_t j = 0; j < colliderComponents.size(); j++)
	{

		BoxColliderComponent* otherBoxColl = colliderComponents[j];

		if (!otherBoxColl->IsActive() || otherBoxColl == boxCollider)
			continue;

		glm::vec3 currentPosition = boxCollider->GetGlobalPosition();
		glm::vec3 extents = boxCollider->GetExtents();

		glm::vec3 otherObjectPosition = otherBoxColl->GetGlobalPosition();
		glm::vec3 otherExtents = otherBoxColl->GetExtents();

		glm::vec3 delta = currentPosition - otherObjectPosition;
		glm::vec3 extentsSum = extents + otherExtents;

		glm::vec3 intersection = glm::vec3(abs(delta.x) - extentsSum.x, abs(delta.y) - extentsSum.y, abs(delta.z) - extentsSum.z);

		if (intersection.x < 0 && intersection.y < 0 && intersection.z < 0)
		{
			glm::vec3 responseVec{ 0.0f, 0.0f, 0.0f };
			glm::vec3 otherResponseVec{ 0.0f, 0.0f, 0.0f };

			if (otherBoxColl->IsTrigger())
			{
				otherBoxColl->OnTrigger();
			}
			else
			{
				if (intersection.x > intersection.y && intersection.x > intersection.z)
				{
					if (delta.x > 0)
					{
						responseVec += glm::vec3(-intersection.x, 0, 0);
						otherResponseVec += glm::vec3(intersection.x, 0, 0);
					}
					else
					{
						responseVec += glm::vec3(intersection.x, 0, 0);
						otherResponseVec += glm::vec3(-intersection.x, 0, 0);
					}
				}
				else if (intersection.y > intersection.z)
				{
					if (delta.y > 0)
					{
						responseVec += glm::vec3(0, -intersection.y, 0);
						otherResponseVec += glm::vec3(0, intersection.y, 0);
					}
					else
					{
						responseVec += glm::vec3(0, intersection.y, 0);
						otherResponseVec += glm::vec3(0, -intersection.y, 0);
					}
				}
				else
				{
					if (delta.z > 0)
					{
						responseVec += glm::vec3(0, 0, -intersection.z);
						otherResponseVec += glm::vec3(0, 0, intersection.z);
					}
					else
					{
						responseVec += glm::vec3(0, 0, intersection.z);
						otherResponseVec += glm::vec3(0, 0, -intersection.z);
					}
				}

				std::cout << boxCollider->GetParent()->name << " is colliding with " << otherBoxColl->GetParent()->name << "\n";

				float d = glm::distance(glm::vec3(0, 0, 0), otherResponseVec);
				std::cout << "d: " << d << "\n";
				if (otherBoxColl->isPushable && d > 0.001f)
				{
					std::cout << boxCollider->GetParent()->name << " push " << otherBoxColl->GetParent()->name << "\n";
					otherBoxColl->GetParent()->Move(otherResponseVec);
				}
				else
				{
					std::cout << otherBoxColl->GetParent()->name << " push " << boxCollider->GetParent()->name << "\n";
					boxCollider->GetParent()->Move(responseVec);
				}
			}
		}

	}
}
