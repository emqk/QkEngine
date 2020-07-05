#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\gtc\quaternion.hpp>

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
	Transform();
	~Transform();

	Transform& operator=(const Transform& other);

	void Translate(const glm::vec3& offset);

	void SetLocalPosition(const glm::vec3& newPosition);
	void SetLocalRotation(const glm::quat& newRotation);
	void SetLocalScale(const glm::vec3& newScale);
	glm::vec3 GetLocalPosition() const;
	glm::quat GetLocalRotation() const;
	glm::vec3 GetLocalScale() const;


	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	
	glm::mat4x4 GetLocalMatrix() const;

	static glm::vec3 ConvertQuaternionToEulerAngles(const glm::quat& quat);
	static glm::vec3 ConvertMatrixToPosition(const glm::mat4& mat);
	static glm::vec3 ConvertMatrixToRotation(const glm::mat4& mat);
	static glm::vec3 ConvertMatrixToScale(const glm::mat4& mat);
	static MatrixDecomposeData DecomposeMatrix(const glm::mat4& matrix);

private:
	void UpdateVectors();

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat localRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 localScale = glm::vec3(1.0f, 1.0f, 1.0f);
};