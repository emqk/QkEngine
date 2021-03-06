#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include <vector>

class GameObject;

struct MatrixDecomposeData
{
	glm::mat4 model;
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
};

class Transform
{
public:
	Transform(GameObject* myRoot);
	~Transform();

	Transform& operator=(const Transform& other);

	bool IsObjectOneOfMyParents(const GameObject const* potentialParent) const;

	void Translate(const glm::vec3& offset);
	void Rotate(const glm::quat& offset);
	void Scale(const glm::vec3& offset);

	const glm::mat4& GetModel() const;

	bool MoveTo(const glm::vec3& position, const float& speed);

	void SetLocalPosition(const glm::vec3& newPosition);
	void SetLocalRotation(const glm::quat& newRotation);
	void SetLocalScale(const glm::vec3& newScale);
	glm::vec3 GetLocalPosition() const;
	glm::quat GetLocalRotation() const;
	glm::vec3 GetLocalScale() const;

	void SetGlobalPosition(const glm::vec3& newPosition);
	void SetGlobalRotation(const glm::quat& newRotation);
	void SetGlobalScale(const glm::vec3& newScale);
	glm::vec3 GetGlobalPosition() const;
	glm::quat GetGlobalRotation() const;
	glm::quat GetGlobalEulerAngles() const;
	glm::vec3 GetGlobalScale() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	static glm::quat ConvertQuaternionToQuaternionEulerAngles(const glm::quat& quat);
	static glm::quat ToQuaternion(const glm::vec3& rotationVec);

	void OnChange();

	const std::vector<GameObject*>& GetChilds() const;
	void SetParent(GameObject* newParent);
	GameObject* GetParent() const;
	void RemoveFromParent();

private:
	void UpdateVectors();
	void UpdateGlobal();
	void SetRoot(GameObject* newRoot);

	void CalculateModel();
	void CalculateModel(glm::mat4 model);

	glm::mat4x4 GetLocalMatrix() const;
	static MatrixDecomposeData DecomposeMatrix(const glm::mat4& matrix);

	glm::mat4 myModel = glm::mat4(1.0f);

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 globalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat globalRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 globalScale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat localRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 localScale = glm::vec3(1.0f, 1.0f, 1.0f);

	GameObject* root = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
};