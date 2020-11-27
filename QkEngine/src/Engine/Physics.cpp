#include "Physics.h"
#include "GameObject.h"
#include "Scene.h"
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

		std::vector<BoxColliderComponent*> colliders;
		CheckCollisionFor(currentBoxColl, colliders);
	}
}

std::vector<BoxColliderComponent*> Physics::BoxCast(const glm::vec3& pos, const glm::vec3& halfExtents)
{
	std::vector<BoxColliderComponent*> result;
	
	for (size_t j = 0; j < colliderComponents.size(); j++)
	{
		BoxColliderComponent* otherBoxColl = colliderComponents[j];
	
		if (!otherBoxColl->IsActive())
			continue;
		
		glm::vec3 otherObjectPosition = otherBoxColl->GetLocalPosition();
		glm::vec3 otherExtents = otherBoxColl->GetExtentsHalf();
	
		glm::vec3 delta = pos - otherObjectPosition;
		glm::vec3 extentsSum = halfExtents + otherExtents;
	
		glm::vec3 intersection = glm::vec3(abs(delta.x) - extentsSum.x, abs(delta.y) - extentsSum.y, abs(delta.z) - extentsSum.z);
	
		if (intersection.x < 0 && intersection.y < 0 && intersection.z < 0)
		{
			result.push_back(otherBoxColl);
		}
	}
	
	return std::move(result);
}

bool Physics::BoxCastCheck(const glm::vec3& pos, const glm::vec3& halfExtents)
{
	for (size_t j = 0; j < colliderComponents.size(); j++)
	{
		BoxColliderComponent* otherBoxColl = colliderComponents[j];

		if (!otherBoxColl->IsActive())
			continue;

		glm::vec3 otherObjectPosition = otherBoxColl->GetLocalPosition();
		glm::vec3 otherExtents = otherBoxColl->GetExtentsHalf();

		glm::vec3 delta = pos - otherObjectPosition;
		glm::vec3 extentsSum = halfExtents + otherExtents;

		glm::vec3 intersection = glm::vec3(abs(delta.x) - extentsSum.x, abs(delta.y) - extentsSum.y, abs(delta.z) - extentsSum.z);

		if (intersection.x < 0 && intersection.y < 0 && intersection.z < 0)
		{
			return true;
		}
	}

	return false;
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

void Physics::CheckCollisionFor(BoxColliderComponent* boxCollider,std::vector<BoxColliderComponent*>& deep)
{
	if (std::find(deep.begin(), deep.end(), boxCollider) == deep.end())
	{
		deep.push_back(boxCollider);
	}
	else
	{
		return;
	}

	for (size_t j = 0; j < colliderComponents.size(); j++)
	{
		BoxColliderComponent* otherBoxColl = colliderComponents[j];

		if (!otherBoxColl->IsActive() || otherBoxColl == boxCollider)
			continue;

		glm::vec3 currentPosition = boxCollider->GetGlobalPosition();
		glm::vec3 extents = boxCollider->GetExtentsHalf();

		glm::vec3 otherObjectPosition = otherBoxColl->GetGlobalPosition();
		glm::vec3 otherExtents = otherBoxColl->GetExtentsHalf();

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

				if (otherBoxColl->isPushable)
				{
					glm::vec3 old = otherBoxColl->GetParent()->transform.GetGlobalPosition();
			
					otherBoxColl->GetParent()->Move(otherResponseVec);
					CheckCollisionFor(otherBoxColl, deep);
				
					float d = glm::distance(old, otherBoxColl->GetParent()->transform.GetGlobalPosition());
					if (d < 0.000001f)
					{
						otherBoxColl->GetParent()->Move(-otherResponseVec);
						boxCollider->GetParent()->Move(responseVec);
					}
				}
				else
				{
					boxCollider->GetParent()->Move(responseVec);
				}
			}
		}

	}

	auto it = std::find(deep.begin(), deep.end(), boxCollider);
	if (it == deep.end())
	{
		deep.erase(it);
	}
}
