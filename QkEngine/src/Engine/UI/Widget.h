#pragma once
#include "../Texture.h"
#include <glm\ext\vector_float2.hpp>


class Widget
{
public:
	Widget();
	Widget(const glm::vec2& _position, const glm::vec2& _size);
	virtual ~Widget();

	void SetTexture(Texture* tex);

	Texture* GetTexture() const;
	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

	virtual void OnPressed();
	virtual void OnPressedCancel();
	virtual void OnReleased();

	virtual void OnCursorEnter();
	virtual void OnCursorExit();

	bool isCursorOn = false;
	bool isPressed = false;

private:
	Texture* texture;

	glm::vec2 position;
	glm::vec2 size;
};