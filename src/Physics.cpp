#include "Physics.h"
#include "GameObject.h"
#include "Components/MoveComponent.h"

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

		for (size_t j = 0; j < colliderComponents.size(); j++)
		{
			BoxColliderComponent* otherBoxColl = colliderComponents[j];

			if (!otherBoxColl->IsActive() || otherBoxColl == currentBoxColl)
				continue;
		
			glm::vec3 currentPosition = currentBoxColl->GetParent()->GetPosition();
			glm::vec3 extents = currentBoxColl->GetExtents();

			glm::vec3 otherObjectPosition = otherBoxColl->GetParent()->GetPosition();
			glm::vec3 otherExtents = otherBoxColl->GetExtents();
		
			glm::vec3 delta = currentPosition - otherObjectPosition;
			glm::vec3 extentsSum = extents + otherExtents;

			glm::vec3 intersection = glm::vec3(abs(delta.x) - extentsSum.x, abs(delta.y) - extentsSum.y, abs(delta.z) - extentsSum.z);

			if (intersection.x < 0 && intersection.y < 0 && intersection.z < 0)
			{
				glm::vec3 responseVec{ 0.0f, 0.0f, 0.0f };
				if (intersection.x > intersection.y && intersection.x > intersection.z)
				{
					if (delta.x > 0)
						responseVec += glm::vec3(-intersection.x, 0, 0);
					else
						responseVec += glm::vec3(intersection.x, 0, 0);
				}
				else if(intersection.y > intersection.z)
				{
					if (delta.y > 0)
						responseVec += glm::vec3(0, -intersection.y, 0);
					else
						responseVec += glm::vec3(0, intersection.y, 0);
				}
				else
				{
					if (delta.z > 0)
						responseVec += glm::vec3(0, 0, -intersection.z);
					else
						responseVec += glm::vec3(0, 0, intersection.z);
				}

				currentBoxColl->GetParent()->SetPosition(currentPosition + responseVec);
			}
		}
	}
}

void Physics::RegisterCollider(BoxColliderComponent* comp)
{
	colliderComponents.push_back(comp);
	std::cout << "BoxColliderComponent registered\n";
}

void Physics::UnRegisterCollider(BoxColliderComponent* comp)
{
    std::vector<BoxColliderComponent*>::iterator it = std::find(colliderComponents.begin(), colliderComponents.end(), comp);
    if (it != colliderComponents.end())
    {
        colliderComponents.erase(it);
        std::cout << "BoxColliderComponent removed from physics: " << colliderComponents.size() << "\n";
    }
}