#include "Transform.h"

#include <glm\trigonometric.hpp>
#include <glm\gtx\matrix_decompose.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


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

void Transform::SetLocalRotation(const glm::quat& newRotation)
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

glm::quat Transform::GetLocalRotation() const
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

glm::vec3 Transform::ConvertQuaternionToEulerAngles(const glm::quat& quat)
{
	return glm::eulerAngles(quat) * 180.0f / (float)M_PI;
}

glm::vec3 Transform::ConvertMatrixToPosition(const glm::mat4& mat)
{
	return mat[3];
}

glm::vec3 Transform::ConvertMatrixToRotation(const glm::mat4& mat)
{
	return mat[2];
}

glm::vec3 Transform::ConvertMatrixToScale(const glm::mat4& mat)
{
	return mat[1];
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

MatrixDecomposeData Transform::DecomposeMatrix(const glm::mat4& matrix)
{
	MatrixDecomposeData data;
	data.model = matrix;
	glm::decompose(data.model, data.scale, data.orientation, data.translation, data.skew, data.perspective);
	return data;
}
