#include "Transform.h"
#include "GameObject.h"

#include <glm\trigonometric.hpp>
#include <glm\gtx\matrix_decompose.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


Transform::Transform(GameObject* myRoot)
{
	SetRoot(myRoot);
}

Transform::~Transform()
{
}

Transform& Transform::operator=(const Transform& other)
{
	myModel = other.myModel;

	globalPosition = other.globalPosition;
	globalRotation = other.globalRotation;
	globalScale = other.globalScale;

	localPosition = other.localPosition;
	localRotation = other.localRotation;
	localScale = other.localScale;

	glm::vec3 Front = other.Front;
	glm::vec3 Right = other.Right;
	glm::vec3 Up = other.Up;

	OnChange();

	return *this;
}

bool Transform::IsObjectOneOfMyParents(const GameObject const* potentialParent) const
{
	GameObject* currObj = root;
	if (currObj->transform.parent == nullptr)
		return false;

	while ((currObj = currObj->transform.parent) != nullptr)
	{
		if (currObj == potentialParent)
		{
			return true;
		}
	}

	return false;
}

void Transform::SetRoot(GameObject* newRoot)
{
	root = newRoot;
	if (root != nullptr)
	{
		SetLocalPosition(root->transform.GetLocalPosition());
		SetLocalRotation(root->transform.GetLocalRotation());
		SetLocalScale(root->transform.GetLocalScale());
	}
	else
		assert("Root of the Transform can't be nullptr!");

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

void Transform::Rotate(const glm::quat& offset)
{
	SetLocalRotation(GetLocalRotation() + offset);
}

void Transform::Scale(const glm::vec3& offset)
{
	SetLocalScale(GetLocalScale() + offset);
}

const glm::mat4& Transform::GetModel() const
{
	return myModel;
}

bool Transform::MoveTo(const glm::vec3& position, const float& speed)
{
	float distToTarget = glm::length(position - GetGlobalPosition());
	if (distToTarget <= speed)
	{
		SetGlobalPosition(position);
		return true;
	}
	else
	{
		glm::vec3 dirToTarget = glm::normalize(position - GetGlobalPosition());
		Translate(dirToTarget * speed);
	}

	return false;
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

void Transform::SetGlobalPosition(const glm::vec3& newPosition)
{
	glm::vec3 thisGlobalToNewPostionDiff = newPosition - GetGlobalPosition();
	glm::vec3 finalGlobalPos = GetLocalPosition() + thisGlobalToNewPostionDiff;
	SetLocalPosition(finalGlobalPos);
}

void Transform::SetGlobalRotation(const glm::quat& newRotation)
{
	if (root->transform.GetParent() == nullptr)
	{
		SetLocalRotation(newRotation);
		return;
	}

	glm::quat parentRot = root->transform.GetParent()->transform.GetGlobalEulerAngles();
	glm::quat targetGlobalRot = glm::quat(1, 0, 0, 0);
	//Rotate by new rotation
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(newRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(newRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(newRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//Rotate by parent rotation
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(parentRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(parentRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	targetGlobalRot = glm::rotate(targetGlobalRot, glm::radians(parentRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	targetGlobalRot = ConvertQuaternionToQuaternionEulerAngles(targetGlobalRot);

	SetLocalRotation(targetGlobalRot);
}

void Transform::SetGlobalScale(const glm::vec3& newScale)
{
	GameObject* obj = root->transform.GetParent();
	if (obj == nullptr)
	{
		SetLocalScale(newScale);
	}
	else
	{
		glm::vec3 targetScale = newScale / obj->transform.GetGlobalScale();
		SetLocalScale(targetScale);
	}
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
	model = glm::rotate(model, glm::radians(-localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, localScale);
	return model;
}

void Transform::CalculateModel()
{
	glm::mat4 model = glm::mat4(1.0f);;
	if (parent == nullptr)
	{
		model = glm::mat4(1.0f);
	}
	else
	{
		model = parent->transform.myModel;
	}
	myModel = model * GetLocalMatrix();

	for (GameObject* child : childs)
	{
		child->transform.CalculateModel(myModel);
	}
}

void Transform::CalculateModel(glm::mat4 model)
{
	model *= GetLocalMatrix();
	myModel = model;

	for (GameObject* child : childs)
	{
		child->transform.CalculateModel(myModel);
	}
}

void Transform::OnChange()
{
	CalculateModel();
	UpdateVectors();
	UpdateGlobal();

}

const std::vector<GameObject*>& Transform::GetChilds() const
{
	return childs;
}

void Transform::SetParent(GameObject* newParent)
{
	//std::cout <<"\t Setting parent of " << root->name << std::endl;
	//It's for staying object at the same position. It works but it moves object to wrong position when Exit GameMode
	//glm::vec3 globalPosBeforeParentChange = GetGlobalPosition();

	if (GetParent() != nullptr)
	{
		if (newParent != nullptr && &newParent->transform == this)
		{
			std::cout << ("Can't add child to the same parent as it's current parent(or nullptr)! [RETURN]");
			return;
		}

		//Remove child from it's current parent
		RemoveFromParent();
	}

	if (newParent != nullptr)
		newParent->transform.childs.push_back(root);

	parent = newParent;
	OnChange();
	
	//It's for staying object at the same position. It works but it moves object to wrong position when Exit GameMode
	//SetGlobalPosition(globalPosBeforeParentChange);
	//std::cout << "\t SetParent targetPos: " << globalPosBeforeParentChange.x << "x " << globalPosBeforeParentChange.y << "y " << globalPosBeforeParentChange.z << "z\n";
	//OnChange();
}

GameObject* Transform::GetParent() const
{
	return parent;
}

void Transform::RemoveFromParent()
{
	if (GetParent() != nullptr)
	{
		//std::cout << "\t Removing " << root->name << " from parent\n";
		std::vector<GameObject*>::iterator it = std::find(parent->transform.childs.begin(), parent->transform.childs.end(), root);
		if (it != parent->transform.childs.end())
			root->transform.parent->transform.childs.erase(it);

		//Null parent and refresh transform
		glm::vec3 gPos = GetGlobalPosition();
		parent = nullptr;
		OnChange();
		SetGlobalPosition(gPos);
	}
	else
	{
		std::cout << "Can't remove from parent - Parent is null!\n";
	}
}

void Transform::UpdateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(localRotation.y - 90.0f)) * cos(glm::radians(localRotation.x));
	front.y = sin(glm::radians(localRotation.x));
	front.z = sin(glm::radians(localRotation.y - 90.0f)) * cos(glm::radians(localRotation.x));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Transform::UpdateGlobal()
{
	if (root != nullptr)
	{
		//glm::mat4 model = CalculateModel(root);
		MatrixDecomposeData decomposedMatrix = DecomposeMatrix(myModel);
		globalPosition = decomposedMatrix.translation;
		globalRotation = decomposedMatrix.orientation;
		globalScale = decomposedMatrix.scale;

		for (GameObject* child : GetChilds())
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