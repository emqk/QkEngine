#pragma once
#include <GLFW/glfw3.h>
#include <vector>

class InputManager
{
public:
	InputManager() = delete;
	~InputManager() = delete;

	static bool GetKey(const int& key);
	static bool GetKeyUp(const int& key);
	static bool GetKeyDown(const int& key);
	static bool GetMouseKeyPressed(const int& key);

	static void Update();

private:
	static std::vector<int> pressedButtons;
	static std::vector<int> pressedButtonsUp;
	static std::vector<int> pressedButtonsDown;
};