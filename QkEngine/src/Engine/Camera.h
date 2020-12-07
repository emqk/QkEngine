#pragma once
#include <GLFW\glfw3.h>
#include <glm\ext\vector_float3.hpp>

#include "Shader.h"
#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(const glm::vec3& pos);
	~Camera();

	glm::mat4 GetMatrix();
	float GetFOV() const;
	glm::vec2 GetClipping() const;

	void SetLocalPosition(const glm::vec3 pos);
	glm::vec3 GetLocalPosition() const;

	void SetLocalRotation(const glm::quat& rot);
	glm::quat GetLocalRotation() const;

	void ShowOnInspector();
	void Update(const float& deltaTime);
	void ReceiveScrollInput(double xoffset, double yoffset);

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void SetClearColor(const glm::vec4& newColor);
	glm::vec4 GetClearColor();

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);


private:
	void ProcessInput(const float& deltaTime);

	Transform transform{ nullptr };

	float movementSpeed = 15.0f;
	float rotationSpeed = 5.0f;

	float FOV = 60;
	float clippingNear = 0.1f;
	float clippingFar = 500;

	glm::vec4 clearColor{ 0.4f, 0.4f, 0.9f, 1.0f };

	bool wasLastFrameMousePressed = false;
};