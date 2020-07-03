#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>

class Transform
{
public:
	Transform();
	~Transform();

	Transform& operator=(const Transform& other);

	void Translate(const glm::vec3& offset);

	void SetLocalPosition(const glm::vec3& newPosition);
	void SetLocalRotation(const glm::vec3& newRotation);
	void SetLocalScale(const glm::vec3& newScale);
	glm::vec3 GetLocalPosition() const;
	glm::vec3 GetLocalRotation() const;
	glm::vec3 GetLocalScale() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	glm::mat4x4 GetLocalMatrix() const;


	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

private:
	void UpdateVectors();

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 localRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 localScale = glm::vec3(1.0f, 1.0f, 1.0f);
};