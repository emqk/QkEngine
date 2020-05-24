#include "InputManager.h"
#include "Window.h"


bool InputManager::GetKeyPressed(const int& key)
{
	return glfwGetKey(&Window::GetCurrentWindow()->GetGLFWWindow(), key) == GLFW_PRESS;
}

bool InputManager::GetMouseKeyPressed(const int& key)
{
	return glfwGetMouseButton(&Window::GetCurrentWindow()->GetGLFWWindow(), key) == GLFW_PRESS;
}