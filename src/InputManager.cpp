#include "InputManager.h"
#include "Window.h"
#include <iostream>

std::vector<int> InputManager::pressedButtons;
std::vector<int> InputManager::pressedButtonsUp;
std::vector<int> InputManager::pressedButtonsDown;

bool InputManager::GetKey(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedButtons.begin(), pressedButtons.end(), key);
	return it != pressedButtons.end();
}

bool InputManager::GetKeyUp(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedButtonsUp.begin(), pressedButtonsUp.end(), key);
	return it != pressedButtonsUp.end();
}

bool InputManager::GetKeyDown(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedButtonsDown.begin(), pressedButtonsDown.end(), key);
	return it != pressedButtonsDown.end();
}

bool InputManager::GetMouseKeyPressed(const int& key)
{
	return glfwGetMouseButton(&Window::GetCurrentWindow()->GetGLFWWindow(), key) == GLFW_PRESS;
}

void InputManager::Update()
{
	pressedButtonsUp.clear();
	pressedButtonsDown.clear();

	for (size_t i = 0; i <= 348; i++)
	{
		int currentKeyState = glfwGetKey(&Window::GetCurrentWindow()->GetGLFWWindow(), i);
		std::vector<int>::iterator it = std::find(pressedButtons.begin(), pressedButtons.end(), i);
		if (currentKeyState == GLFW_PRESS)
		{
			if (it == pressedButtons.end())
			{
				pressedButtons.push_back(i);
				pressedButtonsDown.push_back(i);
			}
		}
		else if (currentKeyState == GLFW_RELEASE)
		{
			if (it != pressedButtons.end())
			{
				pressedButtons.erase(it);
				pressedButtonsUp.push_back(i);
			}
		}
	}
}