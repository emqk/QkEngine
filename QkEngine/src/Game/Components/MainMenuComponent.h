#pragma once
#include "../../Engine/Components/Component.h"
#include "../../Engine/UI/ButtonWidget.h"

class MainMenuComponent : public Component
{
public:
	MainMenuComponent(GameObject* _parent);
	~MainMenuComponent();

	MainMenuComponent(const MainMenuComponent& comp);
	MainMenuComponent operator = (const MainMenuComponent& comp) = delete;

	MainMenuComponent(MainMenuComponent&& comp) = delete;
	MainMenuComponent operator = (MainMenuComponent&& comp) = delete;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;


	void Start() override;

private:
	ButtonWidget* playButtonWidget;
};
