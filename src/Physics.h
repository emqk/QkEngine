#pragma once

#include "Components/BoxColliderComponent.h"
#include <vector>

class Physics
{
public:
	Physics() = delete;
	~Physics() = delete;

	static void Perform();

	static void RegisterCollider(BoxColliderComponent* comp);
	static void UnRegisterCollider(BoxColliderComponent* comp);

private:
	static std::vector<BoxColliderComponent*> colliderComponents;

};