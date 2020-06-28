#include "Transform.h"

#include <glm\trigonometric.hpp>
#include <glm\ext\matrix_transform.hpp>

Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform& Transform::operator=(const Transform& other)
{
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;

	glm::vec3 Front = other.Front;
	glm::vec3 Right = other.Right;
	glm::vec3 Up = other.Up;

	return *this;
}

void Transform::SetPosition(const glm::vec3& newPosition)
{
	position = newPosition;
	UpdateVectors();
}

void Transform::SetRotation(const glm::vec3& newRotation)
{
	rotation = newRotation;
	UpdateVectors();
}

void Transform::SetScale(const glm::vec3& newScale)
{
	scale = newScale;
	UpdateVectors();
}

void Transform::Translate(const glm::vec3& offset)
{
	SetPosition(GetPosition() + offset);
}

glm::vec3 Transform::GetPosition() const
{
	return position;
}

glm::vec3 Transform::GetRotation() const
{
	return rotation;
}

glm::vec3 Transform::GetScale() const
{
	return scale;
}

glm::vec3 Transform::GetFront() const
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

void Transform::UpdateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}