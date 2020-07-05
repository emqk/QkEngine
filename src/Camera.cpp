#include "Camera.h"
#include "InputManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Editor.h"

#include <iostream>
#include <memory>
#include <glm\trigonometric.hpp>
#include <glm\ext\matrix_transform.hpp>

Camera::Camera()
{
	shader = ResourceManager::GetShader("StandardShader");
	SetLocalPosition(glm::vec3(0, 0, 0));
}

Camera::Camera(glm::vec3 pos, const char* shaderPath)
{
	shader = ResourceManager::GetShader(shaderPath);
	SetLocalPosition(pos);
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetMatrix()
{
	return glm::lookAt(transform.GetLocalPosition(), transform.GetLocalPosition() + transform.GetForward(), transform.GetUp());
}

float Camera::GetFOV() const
{
	return FOV;
}

glm::vec2 Camera::GetClipping() const
{
	return glm::vec2(clippingNear, clippingFar);
}


void Camera::SetLocalPosition(const glm::vec3 pos)
{
	transform.SetLocalPosition(pos);
}

glm::vec3 Camera::GetLocalPosition() const
{
	return transform.GetLocalPosition();
}

void Camera::SetLocalRotation(const glm::quat& rot)
{
	transform.SetLocalRotation(rot);
}

glm::quat Camera::GetLocalRotation() const
{
	return transform.GetLocalRotation();
}

void Camera::ShowOnInspector()
{
	//Position
	glm::vec3 pos = GetLocalPosition();
	ImGui::InputFloat3("Position", &pos.x, 3);
	SetLocalPosition(pos);

	//Rotation
	glm::quat localRotation = GetLocalRotation();
	ImGui::InputFloat3("Rotation", &localRotation.x, 3);
	SetLocalRotation(localRotation);
	
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
	if (InputManager::GetMouseKeyDown(GLFW_MOUSE_BUTTON_1))
	{
		if (!Editor::IsAnyWindowOrItemHovered())
		{
			if (objToSelect != nullptr)
			{
				Editor::Select(objToSelect);
			}
		}
	}
}

void Camera::ProcessInput(const float& deltaTime)
{
	if (InputManager::GetMouseKey(GLFW_MOUSE_BUTTON_2))
	{
		//Movement
		glm::vec3 moveVec(0.0f, 0.0f, 0.0f);
		if (InputManager::GetKey(GLFW_KEY_A))
			moveVec += -transform.GetRight();
		if (InputManager::GetKey(GLFW_KEY_D))
			moveVec += transform.GetRight();
	
		if (InputManager::GetKey(GLFW_KEY_W))
			moveVec += transform.GetForward();
		if (InputManager::GetKey(GLFW_KEY_S))
			moveVec += -transform.GetForward();
	
		if (InputManager::GetKey(GLFW_KEY_E))
			moveVec += transform.GetUp();
		if (InputManager::GetKey(GLFW_KEY_Q))
			moveVec += -transform.GetUp();

		transform.Translate(moveVec * movementSpeed * deltaTime);

		//Rotation
		glm::vec2 currMousePos = Scene::GetCurrentScene().GetMousePos();
		if (wasLastFrameMousePressed)
		{
			glm::vec2 diff = currMousePos - previousMousePos;
			SetLocalRotation(GetLocalRotation() + glm::quat(0, -diff.y, diff.x, 0) * rotationSpeed * deltaTime);
		}
		previousMousePos = currMousePos;
		wasLastFrameMousePressed = true;
	}
	else
	{
		wasLastFrameMousePressed = false;
	}
}

void Camera::ReceiveScrollInput(double xoffset, double yoffset)
{
	transform.Translate(transform.GetForward() * (float)yoffset);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ReceiveScrollInput(xoffset, yoffset);
}

Shader& Camera::GetShader()
{
	return *shader;
}
