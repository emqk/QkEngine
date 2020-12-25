#include "MainMenuComponent.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Window.h"

MainMenuComponent::MainMenuComponent(GameObject* _parent) : Component(_parent), playButtonWidget(nullptr), playSecButtonWidget(nullptr)/*, exitButtonWidget(nullptr)*/
{
	name = "MainMenuComponent";
}

MainMenuComponent::~MainMenuComponent()
{
}

MainMenuComponent::MainMenuComponent(const MainMenuComponent& comp) : Component(comp), playButtonWidget(nullptr), playSecButtonWidget(nullptr)/*, exitButtonWidget(nullptr)*/
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
	playButtonWidget = Scene::GetCurrentScene().InstantiateWidget<ButtonWidget>(glm::vec2(0.05f, 0.3f), glm::vec2(0.4f, 0.4f));
	playButtonWidget->SetTextures(
		  ResourceManager::GetTexture("Buttons/Map1Icon.png")
		, ResourceManager::GetTexture("Buttons/Map1Hover.png")
		, ResourceManager::GetTexture("Buttons/Map1Pressed.png"));
	playButtonWidget->SetOnClick([&]()
		{
			Serializer::Deserialize("DayScene");
		}
	);

	playSecButtonWidget = Scene::GetCurrentScene().InstantiateWidget<ButtonWidget>(glm::vec2(0.55f, 0.3f), glm::vec2(0.4f, 0.4f));
	playSecButtonWidget->SetTextures(
		ResourceManager::GetTexture("Buttons/Map2Icon.png")
		, ResourceManager::GetTexture("Buttons/Map2Hover.png")
		, ResourceManager::GetTexture("Buttons/Map2Pressed.png"));
	playSecButtonWidget->SetOnClick([&]()
		{
			Serializer::Deserialize("NightScene");
		}
	);

	//exitButtonWidget = Scene::GetCurrentScene().InstantiateWidget<ButtonWidget>(glm::vec2(0.1f, 0.7f), glm::vec2(0.1f, 0.1f));
	//exitButtonWidget->SetTextures(
	//	ResourceManager::GetTexture("Buttons/ExitIcon.png")
	//	, ResourceManager::GetTexture("Buttons/ExitIconHover.png")
	//	, ResourceManager::GetTexture("Buttons/ExitIconPressed.png"));
	//exitButtonWidget->SetOnClick([&]()
	//	{
	//		Window::GetCurrentWindow()->Close();
	//	}
	//);
}
