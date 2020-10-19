#pragma once
#include "../Component.h"
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

	void SetPath(const std::vector<glm::vec3>& newPath);

protected:
	bool MoveTo(const glm::vec3& position, const float& deltaTime);

private:
	std::vector<glm::vec3> path;
	float movementSpeed = 3;
};