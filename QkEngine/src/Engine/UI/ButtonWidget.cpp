#include "ButtonWidget.h"
#include "../ResourceManager.h"

ButtonWidget::ButtonWidget() : Widget()
	, normalTexture(ResourceManager::GetTexture("Editor/StopIcon.png"))
	, hoverTexture(ResourceManager::GetTexture("Grey.png"))
	, pressedTexture(ResourceManager::GetTexture("Dark.png"))
{
}

ButtonWidget::ButtonWidget(const glm::vec2& _position, const glm::vec2& _size) : Widget(_position, _size)
	, normalTexture(ResourceManager::GetTexture("Editor/StopIcon.png"))
	, hoverTexture(ResourceManager::GetTexture("Grey.png"))
	, pressedTexture(ResourceManager::GetTexture("Dark.png"))
{
}

ButtonWidget::~ButtonWidget()
{
}

void ButtonWidget::OnPressed()
{
	SetTexture(pressedTexture);
	isPressed = true;
}

void ButtonWidget::OnPressedCancel()
{
	SetTexture(normalTexture);
	isPressed = false;
}

void ButtonWidget::OnReleased()
{
	if (onClick)
		onClick();

	SetTexture(hoverTexture);
	isPressed = false;
}

void ButtonWidget::OnCursorEnter()
{
	SetTexture(hoverTexture);
}

void ButtonWidget::OnCursorExit()
{
	SetTexture(normalTexture);
}

void ButtonWidget::SetOnClick(std::function<void()> action)
{
	onClick = action;
}

void ButtonWidget::SetTextures(Texture* newNormalTexture, Texture* newHoverTexture, Texture* newPressedTexture)
{
	SetTexture(newNormalTexture);
	normalTexture = newNormalTexture;
	hoverTexture = newHoverTexture;
	pressedTexture = newPressedTexture;
}
