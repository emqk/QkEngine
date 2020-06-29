#include "InputManager.h"
#include "Window.h"
#include <iostream>

std::vector<int> InputManager::pressedKeyboardButtons;
std::vector<int> InputManager::pressedKeyboardButtonsUp;
std::vector<int> InputManager::pressedKeyboardButtonsDown;

bool InputManager::GetKey(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedKeyboardButtons.begin(), pressedKeyboardButtons.end(), key);
	return it != pressedKeyboardButtons.end();
}

bool InputManager::GetKeyUp(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedKeyboardButtonsUp.begin(), pressedKeyboardButtonsUp.end(), key);
	return it != pressedKeyboardButtonsUp.end();
}

bool InputManager::GetKeyDown(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedKeyboardButtonsDown.begin(), pressedKeyboardButtonsDown.end(), key);
	return it != pressedKeyboardButtonsDown.end();
}

bool InputManager::GetMouseKeyPressed(const int& key)
{
	return glfwGetMouseButton(&Window::GetCurrentWindow()->GetGLFWWindow(), key) == GLFW_PRESS;
}

void InputManager::Update()
{
	pressedKeyboardButtonsUp.clear();
	pressedKeyboardButtonsDown.clear();

	for (size_t i = 0; i <= 348; i++)
	{
		int currentKeyState = glfwGetKey(&Window::GetCurrentWindow()->GetGLFWWindow(), i);
		std::vector<int>::iterator it = std::find(pressedKeyboardButtons.begin(), pressedKeyboardButtons.end(), i);
		if (currentKeyState == GLFW_PRESS)
		{
			if (it == pressedKeyboardButtons.end())
			{
				pressedKeyboardButtons.push_back(i);
				pressedKeyboardButtonsDown.push_back(i);
			}
		}
		else if (currentKeyState == GLFW_RELEASE)
		{
			if (it != pressedKeyboardButtons.end())
			{
				pressedKeyboardButtons.erase(it);
				pressedKeyboardButtonsUp.push_back(i);
			}
		}
	}
}