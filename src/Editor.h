#pragma once
#include <GLFW/glfw3.h>
#include <functional>

#include "GameObject.h"

enum class AssetWindowType
{
	Textures, Meshes, Shaders
};

class Editor
{
public:
	Editor();
	~Editor();

	static void Init(GLFWwindow* window);
	static void Update();
	static void Select(GameObject* obj);

	static void ShowSelectAssetWindow(const AssetWindowType& AssetWindowType, const std::function<void(std::string)>& fun);

private:
	static bool showSelectAssetWindow;
	static AssetWindowType currAssetWindowType;

	static GameObject* selectedObj;
	static Component* selectedComp;
	static std::function<void(const std::string& )> currSelectAssetFun;

	static void EnterGameMode();
	static void ExitGameMode();
};