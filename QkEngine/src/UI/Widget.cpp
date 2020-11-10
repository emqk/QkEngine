#include "Widget.h"
#include "../ResourceManager.h"

Widget::Widget() : position({ 0,0 }), size({ 0.1f, 0.1f }), texture(ResourceManager::GetTexture("Editor/StopIcon.png"))
{}

Widget::Widget(const glm::vec2 & _position, const glm::vec2& _size) : position(_position), size(_size), texture(ResourceManager::GetTexture("Editor/StopIcon.png"))
{}

Widget::~Widget()
{
}

void Widget::SetTexture(Texture* tex)
{
	texture = tex;
}

Texture* Widget::GetTexture() const
{
	return texture;
}

glm::vec2 Widget::GetPosition() const
{
	return position;
}

glm::vec2 Widget::GetSize() const
{
	return size;
}

void Widget::OnPressed()
{
}

void Widget::OnPressedCancel()
{
}

void Widget::OnReleased()
{
}

void Widget::OnCursorEnter()
{
}

void Widget::OnCursorExit()
{
}
