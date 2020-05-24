#include "Camera.h"
#include "InputManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Editor.h"

#include <iostream>
#include <memory>
#include <glm\trigonometric.hpp>
#include <glm\ext\matrix_transform.hpp>

Camera::Camera() : position(glm::vec3(0.0f, 0.0f, 0.0f))
{
	shader = ResourceManager::GetShader("StandardShader");
	UpdateVectors();
}

Camera::Camera(glm::vec3 pos, const char* shaderPath) : position(pos)
{
	shader = ResourceManager::GetShader(shaderPath);
	UpdateVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetMatrix()
{
	return glm::lookAt(position, position + Front, Up);
}

float Camera::GetFOV() const
{
	return FOV;
}

glm::vec2 Camera::GetClipping() const
{
	return glm::vec2(clippingNear, clippingFar);
}


void Camera::SetPosition(const glm::vec3 pos)
{
	position = pos;
	UpdateVectors();
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::SetRotation(const glm::vec3& rot)
{
	rotation = rot;
	UpdateVectors();
}

glm::vec3 Camera::GetRotation() const
{
	return rotation;
}

void Camera::ShowOnInspector()
{
	//Position
	glm::vec3 pos = GetPosition();
	ImGui::InputFloat3("Position", &pos.x, 3);
	SetPosition(pos);

	//Rotation
	glm::vec3 rotation = GetRotation();
	ImGui::InputFloat3("Rotation", &rotation.x, 3);
	SetRotation(rotation);
	
	//FOV
	ImGui::InputFloat("FOV", &FOV, 1, 10);

	//Clipping
	ImGui::Text("Clipping");
	ImGui::InputFloat("Near", &clippingNear, 0.1f, 10.0f);
	ImGui::InputFloat("Far", &clippingFar, 5, 10.0f);
}

void Camera::Update(const float& deltaTime)
{
	static int maxDestroyedObjectsLimit = 0;
	ProcessInput(deltaTime);

	GameObject* objToSelect = Scene::GetCurrentScene().Raycast();
	if (InputManager::GetMouseKeyPressed(GLFW_MOUSE_BUTTON_1))
	{
		if (objToSelect != nullptr)
		{
			Editor::Select(objToSelect);
		}
	}
}

void Camera::ProcessInput(const float& deltaTime)
{
	if (InputManager::GetMouseKeyPressed(GLFW_MOUSE_BUTTON_2))
	{
		//Movement
		glm::vec3 moveVec(0.0f, 0.0f, 0.0f);
		if (InputManager::GetKeyPressed(GLFW_KEY_A))
			moveVec += -Right;
		if (InputManager::GetKeyPressed(GLFW_KEY_D))
			moveVec += Right;
	
		if (InputManager::GetKeyPressed(GLFW_KEY_W))
			moveVec +=  Front;
		if (InputManager::GetKeyPressed(GLFW_KEY_S))
			moveVec += -Front;
	
		if (InputManager::GetKeyPressed(GLFW_KEY_E))
			moveVec +=  Up;
		if (InputManager::GetKeyPressed(GLFW_KEY_Q))
			moveVec += -Up;

		position += moveVec * movementSpeed * deltaTime;

		//Rotation
		glm::vec2 currMousePos = Scene::GetCurrentScene().GetMousePos();
		if (wasLastFrameMousePressed)
		{
			glm::vec2 diff = currMousePos - previousMousePos;
			SetRotation(GetRotation() + glm::vec3(-diff.y, diff.x, 0) * rotationSpeed * deltaTime);
		}
		previousMousePos = currMousePos;
		wasLastFrameMousePressed = true;
	}
	else
	{
		wasLastFrameMousePressed = false;
	}

	UpdateVectors();
}

void Camera::UpdateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ReceiveScrollInput(double xoffset, double yoffset)
{
	position += Front * (float)yoffset;
	UpdateVectors();
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ReceiveScrollInput(xoffset, yoffset);
}

Shader& Camera::GetShader()
{
	return *shader;
}
