#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "Shader.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Serialization/Serialization.h"

class Scene
{
	friend Serializer;

public:
	Scene();
	~Scene();

	void OnLoad();
	void Update(const float& deltaTime, glm::mat4 projection, glm::mat4 view);

	Camera& GetCamera();
	static Scene& GetCurrentScene();

	template<typename T>
	GameObject* FindObjectWithComponent()
	{
		for (const std::unique_ptr<GameObject>& o : objects)
		{
			if (o->GetComponent<T>())
			{
				return o.get();
			}
		}

		return nullptr;
	}


	void InstantiateModel(const Model const* model);

	template<typename T>
	T* Instantiate(const glm::vec3& localPosition)
	{
		//std::unique_ptr<GameObject> newObj = std::make_unique<T>();
		objects.emplace_back(std::make_unique<T>());
		GameObject* newObjPtr = objects.back().get();
		newObjPtr->transform.SetLocalPosition(localPosition);
		std::cout << "[GAME_OBJECT INSTANTIATE] Object size: " << objects.size() << "\n";
		return newObjPtr;
	}
	template<typename T>
	T* InstantiateWidget(const glm::vec2& position, const glm::vec2& size)
	{
		widgets.emplace_back(std::make_unique<T>(position, size));
		std::cout << "[WIDGET INSTANTIATE] Widget Object size: " << widgets.size() << "\n";
		return reinterpret_cast<T*>(widgets.back().get());
	}

	GameObject* DuplicateGameObject(const GameObject* obj);


	void Destroy(GameObject* obj);

	void SetMousePos(const glm::vec2& pos, int w, int h);
	glm::vec2 GetMousePos() const;

	GameObject* Raycast();

	const std::vector<std::unique_ptr<GameObject>>* const GetObjectsPtr() const;
	size_t GetNumberOfWidgets() const;

	static const bool& IsInGameMode();
	void EnterGameMode();
	void ExitGameMode();

	void InvokeStart();
	void ClearWidgets();

private:
	static Scene* currentScene;
	static bool inGameMode;

	void UpdateWidgetInteraction();

	void DrawGizmos();

	void DestroyPostponed();
	void DuplicateChilds(const GameObject* obj, GameObject* parent);
	void DuplicateChildsCopy(GameObject* obj, GameObject* parent);

	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> objectsCopy; //In game mode
	std::vector<bool> objectsCopyActiveData; //In game mode
	std::vector<GameObject*> objectsToDestroy;

	std::vector<std::unique_ptr<Widget>> widgets;

	Camera camera;

	glm::mat4 projection;
	glm::mat4 view;

	glm::vec2 mousePos;
	int winWidth;
	int winHeight;
};