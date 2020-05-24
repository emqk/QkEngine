#pragma once
#include <GLFW/glfw3.h>

class InputManager
{
public:
	InputManager() = delete;
	~InputManager() = delete;

	static bool GetKeyPressed(const int& key);
	static bool GetMouseKeyPressed(const int& key);

private:

};