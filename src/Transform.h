#pragma once
#include <glm\ext\vector_float3.hpp>

class Transform
{
public:
	Transform();
	~Transform();

	Transform& operator=(const Transform& other);

	void SetPosition(const glm::vec3& newPosition);
	void SetRotation(const glm::vec3& newRotation);
	void SetScale(const glm::vec3& newScale);

	void Translate(const glm::vec3& offset);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;


	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

private:
	void UpdateVectors();

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};