#include "GameManager.h"

#include "../Scene.h"
#include "../ResourceManager.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/PlayerComponent.h"
#include <iostream>

GameManager::GameManager()
{
	//Terrain
	GameObject* terrainObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	terrainObj->name = "Ground";
	SpriteComponent* terrainSprite = terrainObj->AddComponent<SpriteComponent>();
	if (terrainSprite != nullptr)
	{
		terrainSprite->SetMeshNew("Terrain.obj");
		terrainSprite->SetTexture(ResourceManager::GetTexture("Terrain_Albedo.jpg"));
		terrainSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	
		glm::vec3 terrainBounds = terrainSprite->GetMeshNew()->GetBounds().Extents();
		terrainObj->SetPosition(glm::vec3(0.0f, -terrainBounds.y, -terrainBounds.z));
	}
	terrainObj->AddComponent<BoxColliderComponent>();

	float currX = 0;
	float currY = 2;
	float currZ = -4;
	for (glm::vec3& pos : buildAreasPos)
	{
		pos = glm::vec3(currX, currY, currZ);
		currX += 4;
		if ((int)currX % 80 == 0)
		{
			currX = 0;
			currY += 4;
		}
	}

	for (size_t i = 0; i < buildAreasPos.size(); i++)
	{
		GameObject* instance = Scene::GetCurrentScene().Instantiate<GameObject>(buildAreasPos[i]);
		instance->name = "BuildArea";
		SpriteComponent* areaSprite = instance->AddComponent<SpriteComponent>();
		if (areaSprite != nullptr)
		{
			areaSprite->SetMeshNew("Buiilding Area.obj");
			areaSprite->SetTexture(ResourceManager::GetTexture("Building Area 1_Albedo.jpg"));
			areaSprite->SetShader(ResourceManager::GetShader("StandardShader"));
		}

		instance->AddComponent<BoxColliderComponent>();

		buildAreas[i] = instance;
	}

	//Player
	GameObject* playerObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(-3, 1, -2));
	PlayerComponent* playerComp = playerObj->AddComponent<PlayerComponent>();
	playerObj->name = "Player";


	GameObject* stairs = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(4, 2.5f, -1));
	stairs->name = "Stairs";
	SpriteComponent* stairsSprite = stairs->AddComponent<SpriteComponent>();
	if (stairsSprite != nullptr)
	{
		stairsSprite->SetMeshNew("Stairs.obj");
		stairsSprite->SetTexture(ResourceManager::GetTexture("Stairs.jpg"));
		stairsSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	}
}

GameManager::~GameManager()
{

}