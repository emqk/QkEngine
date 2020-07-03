#pragma once
#include <GLFW\glfw3.h>
#include <glm\ext\vector_float3.hpp>

#include "Shader.h"
#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, const char* shaderPath);
	~Camera();

	glm::mat4 GetMatrix();
	float GetFOV() const;
	glm::vec2 GetClipping() const;

	void SetLocalPosition(const glm::vec3 pos);
	glm::vec3 GetLocalPosition() const;

	void SetLocalRotation(const glm::vec3& rot);
	glm::vec3 GetLocalRotation() const;

	void ShowOnInspector();
	void Update(const float& deltaTime);
	void ReceiveScrollInput(double xoffset, double yoffset);

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	Shader& GetShader();

private:
	void ProcessInput(const float& deltaTime);

	Transform transform;

	float movementSpeed = 15.0f;
	float rotationSpeed = 5.0f;

	float FOV = 60;
	float clippingNear = 0.1f;
	float clippingFar = 500;

	Shader* shader;

	glm::vec2 previousMousePos;
	bool wasLastFrameMousePressed = false;
};