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
		terrainSprite->SetMeshNew("Terrain.obj->Cube");
		terrainSprite->SetTexture(ResourceManager::GetTexture("Terrain_Albedo.jpg"));
		terrainSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	
		glm::vec3 terrainBounds = terrainSprite->GetMeshNew()->GetBounds().Extents();
		terrainObj->SetLocalPosition(glm::vec3(0.0f, -terrainBounds.y, -terrainBounds.z));
	}
	terrainObj->AddComponent<BoxColliderComponent>();

	//float currX = 0;
	//float currY = 2;
	//float currZ = -4;
	//for (glm::vec3& pos : buildAreasPos)
	//{
	//	pos = glm::vec3(currX, currY, currZ);
	//	currX += 4;
	//	if ((int)currX % 80 == 0)
	//	{
	//		currX = 0;
	//		currY += 4;
	//	}
	//}

	//for (size_t i = 0; i < buildAreasPos.size(); i++)
	//{
	//	GameObject* instance = Scene::GetCurrentScene().Instantiate<GameObject>(buildAreasPos[i]);
	//	instance->name = "BuildArea";
	//	SpriteComponent* areaSprite = instance->AddComponent<SpriteComponent>();
	//	if (areaSprite != nullptr)
	//	{
	//		areaSprite->SetMeshNew("Buiilding Area.obj");
	//		areaSprite->SetTexture(ResourceManager::GetTexture("Building Area 1_Albedo.jpg"));
	//		areaSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	//	}

	//	instance->AddComponent<BoxColliderComponent>();

	//	buildAreas[i] = instance;
	//}

	//Player
	playerObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 5, 0));
	PlayerComponent* playerComp = playerObj->AddComponent<PlayerComponent>();
	playerObj->name = "Player";


	GameObject* newParent = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	SpriteComponent* sprComp = newParent->AddComponent<SpriteComponent>();
	sprComp->SetMeshNew("Cube.obj->Cube");
	sprComp->SetTexture(ResourceManager::GetTexture("Cube.jpg"));
	sprComp->SetShader("StandardShader");
	newParent->name = "TestChairParent";

	//Chair
	GameObject* chairObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(2, 2, 2));
	SpriteComponent* chairSprite = chairObj->AddComponent<SpriteComponent>();
	chairSprite->SetMeshNew("Chair/School Chair Offset.fbx->Cylinder.006");
	chairSprite->SetTexture(ResourceManager::GetTexture("Chair/diffuse.png"));
	chairSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	chairObj->AddComponent<BoxColliderComponent>();
	chairObj->name = "First child";
	newParent->AddChild(chairObj);

	GameObject* sec = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(2, 0, 0));
	SpriteComponent* secSprComp = sec->AddComponent<SpriteComponent>();
	secSprComp->SetMeshNew("Chair/School Chair Offset.fbx->Cylinder.006");
	secSprComp->SetTexture(ResourceManager::GetTexture("Chair/diffuse.png"));
	secSprComp->SetShader("StandardShader");
	sec->name = "Second child";
	chairObj->AddChild(sec);

	//GameObject* stairs = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(4, 2.5f, -1));
	//stairs->name = "Stairs";
	//SpriteComponent* stairsSprite = stairs->AddComponent<SpriteComponent>();
	//if (stairsSprite != nullptr)
	//{
	//	stairsSprite->SetMeshNew("Stairs.obj");
	//	stairsSprite->SetTexture(ResourceManager::GetTexture("Stairs.jpg"));
	//	stairsSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	//}
}

GameManager::~GameManager()
{

}