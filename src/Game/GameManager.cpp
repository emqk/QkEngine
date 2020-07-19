#include "GameManager.h"

#include "../Scene.h"
#include "../ResourceManager.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/LightComponent.h"
#include "../Random.h"

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
		terrainObj->transform.SetLocalPosition(glm::vec3(0.0f, -terrainBounds.y, -terrainBounds.z));
	}
	terrainObj->AddComponent<BoxColliderComponent>();

	//Sun
	GameObject* sunObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	sunObj->name = "Sun";
	SpriteComponent* sunSprite = sunObj->AddComponent<SpriteComponent>();
	if (sunSprite != nullptr)
	{
		sunSprite->SetMeshNew("Arrow.fbx->Cone");
		sunSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
		sunSprite->SetShader(ResourceManager::GetShader("StandardShader"));

		sunObj->transform.SetLocalPosition(glm::vec3(0.0f, 6.0f, 0.0f));
	}
	sunObj->AddComponent<LightComponent>();
	sunObj->transform.SetLocalScale(glm::vec3(0.25f));

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
	playerObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 9, 0));
	PlayerComponent* playerComp = playerObj->AddComponent<PlayerComponent>();
	playerObj->name = "Player";

	//Chairs parent
	GameObject* newParent = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	SpriteComponent* sprComp = newParent->AddComponent<SpriteComponent>();
	sprComp->SetMeshNew("Cube.obj->Cube");
	sprComp->SetTexture(ResourceManager::GetTexture("Cube.jpg"));
	sprComp->SetShader("StandardShader");
	newParent->AddComponent<BoxColliderComponent>();
	newParent->name = "TestChairParent";

	//Chair (Child 0)
	GameObject* chairObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(2, 2, 2));
	SpriteComponent* chairSprite = chairObj->AddComponent<SpriteComponent>();
	chairSprite->SetMeshNew("Chair/School Chair Offset.fbx->Cylinder.006");
	chairSprite->SetTexture(ResourceManager::GetTexture("Chair/diffuse.png"));
	chairSprite->SetShader(ResourceManager::GetShader("StandardShader"));
	chairObj->AddComponent<BoxColliderComponent>();
	chairObj->name = "First child";
	chairObj->transform.SetParent(newParent);
	//newParent->transform.AddChild(chairObj);

	//Chair (Child 1)
	GameObject* sec = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(2, 0, 0));
	SpriteComponent* secSprComp = sec->AddComponent<SpriteComponent>();
	secSprComp->SetMeshNew("Chair/School Chair Offset.fbx->Cylinder.006");
	secSprComp->SetTexture(ResourceManager::GetTexture("Chair/diffuse.png"));
	secSprComp->SetShader("StandardShader");
	sec->AddComponent<BoxColliderComponent>();
	sec->name = "Second child";
	sec->transform.SetParent(chairObj);
	//sec->transform.SetParent(playerObj);

	//Duplicate some objects
	for (size_t i = 0; i < 2000; i++)
	{
		GameObject* duplicate = Scene::GetCurrentScene().DuplicateGameObject(newParent);
		float px = Random::RandomFloat(-35, 35);
		float py = 0;
		float pz = Random::RandomFloat(-25, 0);

		float ry = Random::RandomFloat(0, 360);

		duplicate->transform.SetGlobalPosition(glm::vec3(px, py, pz));
		duplicate->transform.SetGlobalRotation(Transform::ToQuaternion(glm::vec3(0, ry, 0)));
	}


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