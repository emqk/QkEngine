#pragma once
#include "../Component.h"
#include "../../Navigation/NavMeshNode.h"
#include <glm\ext\vector_float3.hpp>
#include <vector>

class NavMeshAgentComponent : public Component
{
public:
	NavMeshAgentComponent(GameObject* _parent);
	~NavMeshAgentComponent();

	NavMeshAgentComponent(const NavMeshAgentComponent& comp);
	NavMeshAgentComponent& operator=(const NavMeshAgentComponent& comp) = delete;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;

	void SetPath(const std::vector<NavMeshNode*>& newPath);

protected:
	bool FollowPath(const float& deltaTime);
	bool MoveTo(const glm::vec3& position, const float& speed);

	float movementSpeed = 3;

private:
	std::vector<NavMeshNode*> path;
	int currentPathIndex = 0;
};