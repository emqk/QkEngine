#pragma once
#include "../../Engine/Components/Component.h"

class PlatformComponent : public Component
{
public:
	PlatformComponent(GameObject* _parent);
	~PlatformComponent();
	
	PlatformComponent(const PlatformComponent& comp);
	PlatformComponent& operator=(const PlatformComponent& comp) = delete;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;


	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;

private:

};
