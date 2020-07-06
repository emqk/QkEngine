#include "Transform.h"
#include "GameObject.h"

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
	globalPosition = other.globalPosition;
	globalRotation = other.globalRotation;
	globalScale = other.globalScale;

	localPosition = other.localPosition;
	localRotation = other.localRotation;
	localScale = other.localScale;

	glm::vec3 Front = other.Front;
	glm::vec3 Right = other.Right;
	glm::vec3 Up = other.Up;

	SetRoot(other.root);

	return *this;
}

void Transform::SetRoot(GameObject* newRoot)
{
	root = newRoot;
	OnChange();
}

void Transform::SetLocalPosition(const glm::vec3& newPosition)
{
	localPosition = newPosition;
	OnChange();

}

void Transform::SetLocalRotation(const glm::quat& newRotation)
{
	localRotation = newRotation;
	OnChange();

}

void Transform::SetLocalScale(const glm::vec3& newScale)
{
	localScale = newScale;
	OnChange();
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

glm::vec3 Transform::GetGlobalPosition() const
{
	return globalPosition;
}

glm::quat Transform::GetGlobalRotation() const
{
	return globalRotation;
}

glm::quat Transform::GetGlobalEulerAngles() const
{
	return ConvertQuaternionToQuaternionEulerAngles(GetGlobalRotation());
}

glm::vec3 Transform::GetGlobalScale() const
{
	return globalScale;
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

glm::quat Transform::ConvertQuaternionToQuaternionEulerAngles(const glm::quat& quat)
{
	return -glm::quat(1, glm::eulerAngles(quat) * 180.0f / (float)M_PI);
}

glm::quat Transform::ToQuaternion(const glm::vec3& rotationVec)
{
	return glm::quat(1, rotationVec);
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

glm::mat4 Transform::CalculateModel(const GameObject const* obj)
{
	glm::mat4 model = glm::mat4(1.0f);
	CalculateModel(obj, model);
	return model;
}

void Transform::CalculateModel(const GameObject const* obj, glm::mat4& model)
{
	if (obj->GetParent() != nullptr)
	{
		CalculateModel(obj->GetParent(), model);
		model *= obj->GetTransform().GetLocalMatrix();
	}
	else
	{
		model = obj->GetTransform().GetLocalMatrix();
	}
}

void Transform::OnChange()
{
	UpdateVectors();
	UpdateGlobal();
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

void Transform::UpdateGlobal()
{
	if (root != nullptr)
	{
		glm::mat4 model = CalculateModel(root);
		MatrixDecomposeData decomposedMatrix = DecomposeMatrix(model);
		globalPosition = decomposedMatrix.translation;
		globalRotation = decomposedMatrix.orientation;
		globalScale = decomposedMatrix.scale;

		for (GameObject* child : root->GetChilds())
		{
			child->transform.UpdateGlobal();
		}
	}
}

MatrixDecomposeData Transform::DecomposeMatrix(const glm::mat4& matrix)
{
	MatrixDecomposeData data;
	data.model = matrix;
	glm::decompose(data.model, data.scale, data.orientation, data.translation, data.skew, data.perspective);
	data.orientation = glm::conjugate(data.orientation);
	return data;
}
