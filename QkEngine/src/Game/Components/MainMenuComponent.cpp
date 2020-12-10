#include "MainMenuComponent.h"
#include "../../Engine/Scene.h"

MainMenuComponent::MainMenuComponent(GameObject* _parent) : Component(_parent), playButtonWidget(nullptr)
{
}

MainMenuComponent::~MainMenuComponent()
{
}

MainMenuComponent::MainMenuComponent(const MainMenuComponent& comp) : Component(comp), playButtonWidget(nullptr)
{
}

std::unique_ptr<Component> MainMenuComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<MainMenuComponent> comp = std::make_unique<MainMenuComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void MainMenuComponent::Start()
{
	playButtonWidget = Scene::GetCurrentScene().InstantiateWidget<ButtonWidget>(glm::vec2(0.1f, 0.1f), glm::vec2(0.2f, 0.1f));
	playButtonWidget->SetOnClick([&]()
		{
			Serializer::Deserialize("DayScene");
		}
	);
}
