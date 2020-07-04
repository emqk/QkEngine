#include "Transform.h"

#include <glm\trigonometric.hpp>


Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform& Transform::operator=(const Transform& other)
{
	localPosition = other.localPosition;
	localRotation = other.localRotation;
	localScale = other.localScale;

	glm::vec3 Front = other.Front;
	glm::vec3 Right = other.Right;
	glm::vec3 Up = other.Up;

	return *this;
}

void Transform::SetLocalPosition(const glm::vec3& newPosition)
{
	localPosition = newPosition;
	UpdateVectors();
}

void Transform::SetLocalRotation(const glm::vec3& newRotation)
{
	localRotation = newRotation;
	UpdateVectors();
}

void Transform::SetLocalScale(const glm::vec3& newScale)
{
	localScale = newScale;
	UpdateVectors();
}

void Transform::Translate(const glm::vec3& offset)
{
	SetLocalPosition(GetLocalPosition() + offset);
}

glm::vec3 Transform::GetLocalPosition() const
{
	return localPosition;
}

glm::vec3 Transform::GetLocalRotation() const
{
	return localRotation;
}

glm::vec3 Transform::GetLocalScale() const
{
	return localScale;
}

glm::vec3 Transform::GetForward() const
{
	return Front;
}

glm::vec3 Transform::GetRight() const
{
	return Right;
}

glm::vec3 Transform::GetUp() const
{
	return Up;
}

glm::mat4x4 Transform::GetLocalMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, localPosition);
	model = glm::rotate(model, glm::radians(localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, localScale);
	return model;
}

void Transform::UpdateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(localRotation.y)) * cos(glm::radians(localRotation.x));
	front.y = sin(glm::radians(localRotation.x));
	front.z = sin(glm::radians(localRotation.y)) * cos(glm::radians(localRotation.x));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}