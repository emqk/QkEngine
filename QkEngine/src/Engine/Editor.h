#pragma once
#include <GLFW/glfw3.h>
#include <functional>

#include "GameObject.h"

enum class AssetWindowType
{
	Textures, Shaders, MeshesNew, Components
};

enum class ObjectTransformMode
{
	None, Position, Rotation, Scale
};

enum class ObjectTransformType
{
	None, X, Y, Z
};

class Editor
{
public:
	Editor() = delete;
	~Editor() = delete;

	static void Init(GLFWwindow* window);
	static void Update();
	static void ShowGameObject(GameObject* obj, int& id, int& node_clicked);
	static void Select(GameObject* obj);

	static bool CanDrawGizmos();
	static bool IsMouseOverViewport();
	static void DrawSelectedGameObject();

	static void ShowSelectAssetWindow(const AssetWindowType& AssetWindowType, const std::function<void(std::string)>& fun);

	static glm::vec2 GetViewportPosition();
	static glm::vec2 GetViewportSize();

	static const std::vector<std::string> availableComponents;

private:
	static void EnableDockingBackground();
	static void ShowEnabledWindows();
	static void ShowInspector();
	static void ShowHierarchy();
	static void ShowSelectAssetWindow();

	static glm::vec2 viewportPosition;
	static glm::vec2 viewportSize;

	static void Drag(GameObject* obj);
	static void NullCurrentlyDraggedHierarchyObj();

	static GameObject* currentlyDraggedHierarchyObj;

	static bool drawGizmos;
	static AssetWindowType currAssetWindowType;

	static bool isMouseOverViewport;

	static GameObject* selectedObj;
	static Component* selectedComp;
	static std::function<void(const std::string& )> currSelectAssetFun;

	static void EnterGameMode();
	static void ExitGameMode();

	static void UpdateGizmosTransformations();

	static Texture* playButtonTex;
	static Texture* stopButtonTex;

	//Windows activity
	static bool showHierarchy;
	static bool showInspector;
	static bool showLightingWindow;
	static bool showProfiler;
	static bool showNavigationWindow;
	static bool showSelectAssetWindow;
	static bool showCameraWindow;

	//Object transformations
	static ObjectTransformMode currentObjectTransformMode;
	static ObjectTransformType currentObjectTransformType;
};