#pragma once
#include <functional>
#include "Widget.h"

class ButtonWidget : public Widget
{
public:
	ButtonWidget();
	ButtonWidget(const glm::vec2& _position, const glm::vec2& _size);
	~ButtonWidget();

	virtual void OnPressed() override;
	virtual void OnPressedCancel() override;
	virtual void OnReleased() override;

	virtual void OnCursorEnter() override;
	virtual void OnCursorExit() override;

	void SetOnClick(std::function<void()> action);

private:
	Texture* normalTexture;
	Texture* hoverTexture;
	Texture* pressedTexture;

	std::function<void()> onClick;
};