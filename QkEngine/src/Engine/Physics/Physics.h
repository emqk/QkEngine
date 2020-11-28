#pragma once

#include "../Components/BoxColliderComponent.h"
#include "Ray.h"
#include <vector>

class Physics
{
public:
	Physics() = delete;
	~Physics() = delete;

	static void Perform();

	static std::vector<BoxColliderComponent*> BoxCast(const glm::vec3& pos, const glm::vec3& halfExtents);
	static bool BoxCastCheck(const glm::vec3& pos, const glm::vec3& halfExtents);

	static void RegisterCollider(BoxColliderComponent* comp);
	static void UnRegisterCollider(BoxColliderComponent* comp);

	static GameObject* RaycastMesh(const Ray& ray);
	static GameObject* RaycastBoxCollider(const Ray& ray);
	static bool Raycast(const Ray& ray, Bounds bounds, glm::vec3 pos);

protected:
	static void CheckCollisionFor(BoxColliderComponent* boxCollider, std::vector<BoxColliderComponent*>& deep);

private:
	static std::vector<BoxColliderComponent*> colliderComponents;

};