#pragma once
#include <GLFW\glfw3.h>
#include <glm\ext\vector_float3.hpp>

#include "Shader.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, const char* shaderPath);
	~Camera();

	glm::mat4 GetMatrix();
	float GetFOV() const;
	glm::vec2 GetClipping() const;

	void SetPosition(const glm::vec3 pos);
	glm::vec3 GetPosition() const;

	void SetRotation(const glm::vec3& rot);
	glm::vec3 GetRotation() const;

	void ShowOnInspector();
	void Update(const float& deltaTime);
	void ReceiveScrollInput(double xoffset, double yoffset);

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	Shader& GetShader();

private:
	void ProcessInput(const float& deltaTime);
	void UpdateVectors();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	float movementSpeed = 15.0f;
	float rotationSpeed = 5.0f;

	float FOV = 60;
	float clippingNear = 0.1f;
	float clippingFar = 500;

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Shader* shader;

	glm::vec2 previousMousePos;
	bool wasLastFrameMousePressed = false;
};