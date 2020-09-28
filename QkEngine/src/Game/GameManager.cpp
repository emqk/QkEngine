#include "GameManager.h"

#include "../Scene.h"
#include "../ResourceManager.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/Lighting/DirectionalLightComponent.h"
#include "../Components/Lighting/PointLightComponent.h"
#include "../Random.h"

#include <iostream>

GameManager::GameManager()
{
	//Sun
	GameObject* sunObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	sunObj->name = "Sun";
	StaticMeshComponent* sunSprite = sunObj->AddComponent<StaticMeshComponent>();
	if (sunSprite != nullptr)
	{
		sunSprite->SetMeshNew("Arrow.fbx->Cone");
		sunSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
		sunSprite->SetShader(ResourceManager::GetShader("StandardShader"));

		sunObj->transform.SetLocalPosition(glm::vec3(0.0f, 6.0f, 0.0f));
	}
	sunObj->AddComponent<DirectionalLightComponent>();
	sunObj->transform.SetLocalScale(glm::vec3(0.25f));

	//Player
	playerObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 9, 0));
	PlayerComponent* playerComp = playerObj->AddComponent<PlayerComponent>();
	playerObj->name = "Player";
}

GameManager::~GameManager()
{

}