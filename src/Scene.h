#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "Shader.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "NewModel.h"

class Scene
{
public:
	Scene();
	~Scene();

	void OnLoad();
	void Update(const float& deltaTime, class Shader& camShader, glm::mat4 projection, glm::mat4 view);

	Camera& GetCamera();
	static Scene& GetCurrentScene();

	void InstantiateModel(const Model const* model);

	template<typename T>
	T* Instantiate(const glm::vec3& localPosition)
	{
		//std::unique_ptr<GameObject> newObj = std::make_unique<T>();
		objects.emplace_back(std::make_unique<T>());
		GameObject* newObjPtr = objects.back().get();
		newObjPtr->SetLocalPosition(localPosition);
		std::cout << "[INSTANTIATE] Object size: " << objects.size() << "\n";
		return newObjPtr;
	}
	GameObject* DuplicateGameObject(const GameObject* obj);

	void Destroy(GameObject* obj);

	void SetMousePos(const glm::vec2& pos, int w, int h);
	glm::vec2 GetMousePos() const;

	GameObject* Raycast();

	const std::vector<std::unique_ptr<GameObject>>* const GetObjectsPtr() const;

	static const bool& IsInGameMode();
	void EnterGameMode();
	void ExitGameMode();

private:
	static void RemoveFromParent(GameObject* child);

	static Scene* currentScene;
	static bool inGameMode;

	void DestroyPostponed();
	GameObject* DuplicateChilds(const GameObject* obj, GameObject* parent);

	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> objectsCopy; //In game mode
	std::vector<bool> objectsCopyActiveData; //In game mode
	std::vector<GameObject*> objectsToDestroy;

	Camera camera;

	glm::mat4 projection;
	glm::mat4 view;

	glm::vec2 mousePos;
	int winWidth = 800;
	int winHeight = 600;
};