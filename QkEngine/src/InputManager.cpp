#include "InputManager.h"
#include "Window.h"
#include <iostream>

std::vector<int> InputManager::pressedKeyboardButtons;
std::vector<int> InputManager::pressedKeyboardButtonsUp;
std::vector<int> InputManager::pressedKeyboardButtonsDown;

std::vector<int> InputManager::pressedMouseButtons;
std::vector<int> InputManager::pressedMouseButtonsUp;
std::vector<int> InputManager::pressedMouseButtonsDown;

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

bool InputManager::GetMouseKey(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedMouseButtons.begin(), pressedMouseButtons.end(), key);
	return it != pressedMouseButtons.end();
}

bool InputManager::GetMouseKeyUp(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedMouseButtonsUp.begin(), pressedMouseButtonsUp.end(), key);
	return it != pressedMouseButtonsUp.end();
}

bool InputManager::GetMouseKeyDown(const int& key)
{
	std::vector<int>::iterator it = std::find(pressedMouseButtonsDown.begin(), pressedMouseButtonsDown.end(), key);
	return it != pressedMouseButtonsDown.end();
}

void InputManager::Update()
{
	pressedKeyboardButtonsUp.clear();
	pressedKeyboardButtonsDown.clear();
	pressedKeyboardButtons.clear();

	pressedMouseButtonsUp.clear();
	pressedMouseButtonsDown.clear();
	pressedMouseButtons.clear();

	//Keyboard
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

	//Mouse
	for (size_t i = 0; i <= 7; i++)
	{
		int currentKeyState = glfwGetMouseButton(&Window::GetCurrentWindow()->GetGLFWWindow(), i);
		std::vector<int>::iterator it = std::find(pressedMouseButtons.begin(), pressedMouseButtons.end(), i);
		if (currentKeyState == GLFW_PRESS)
		{
			if (it == pressedMouseButtons.end())
			{
				pressedMouseButtons.push_back(i);
				pressedMouseButtonsDown.push_back(i);
			}
		}
		else if (currentKeyState == GLFW_RELEASE)
		{
			if (it != pressedMouseButtons.end())
			{
				pressedMouseButtons.erase(it);
				pressedMouseButtonsUp.push_back(i);
			}
		}
	}
}