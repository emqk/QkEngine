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
	static bool GetMouseKey(const int& key);
	static bool GetMouseKeyUp(const int& key);
	static bool GetMouseKeyDown(const int& key);

	static void Update();

private:
	static std::vector<int> pressedKeyboardButtons;
	static std::vector<int> pressedKeyboardButtonsUp;
	static std::vector<int> pressedKeyboardButtonsDown;

	static std::vector<int> pressedMouseButtons;
	static std::vector<int> pressedMouseButtonsUp;
	static std::vector<int> pressedMouseButtonsDown;
};