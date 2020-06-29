#pragma once
#include <GLFW/glfw3.h>
#include <vector>

class InputManager
{
public:
	InputManager() = delete;
	~InputManager() = delete;

	//Keyboard
	static bool GetKey(const int& key);
	static bool GetKeyUp(const int& key);
	static bool GetKeyDown(const int& key);

	//Mouse
	static bool GetMouseKeyPressed(const int& key);

	static void Update();

private:
	static std::vector<int> pressedKeyboardButtons;
	static std::vector<int> pressedKeyboardButtonsUp;
	static std::vector<int> pressedKeyboardButtonsDown;
};