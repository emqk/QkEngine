#include "GameManager.h"

#include "../Scene.h"
#include "../ResourceManager.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/Lighting/DirectionalLightComponent.h"
#include "../Components/Lighting/PointLightComponent.h"
#include "../Components/AI/NavMeshAgent.h"
#include "../Random.h"

#include <iostream>


GameManager::GameManager()
{
	//Sun
	GameObject* sunObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
	sunObj->transform.SetGlobalRotation(glm::quat(1, -30, 20, 0));
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

	//Ground
	const float worldSize = 40;
	GameObject* ground = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, -1.25f, 0));
	StaticMeshComponent* groundSprite = ground->AddComponent<StaticMeshComponent>();
	if (groundSprite != nullptr)
	{
		groundSprite->SetMeshNew("Cube.obj->Cube");
		groundSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
		groundSprite->SetShader(ResourceManager::GetShader("StandardShader"));
		groundSprite->color = glm::vec4(0.4f, 0.85f, 0.4f, 1);
	}
	ground->transform.SetGlobalScale(glm::vec3(worldSize, 2, worldSize));
	BoxColliderComponent* groundCollider = ground->AddComponent<BoxColliderComponent>();
	groundCollider->SetExtents(glm::vec3(worldSize, 2, worldSize));

	//Trees
	for (size_t i = 0; i < 10; i++)
	{
		GameObject* treeBase = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(Random::RandomFloat(-worldSize / 2, worldSize / 2), 0, Random::RandomFloat(-worldSize / 2, worldSize / 2)));
		BoxColliderComponent* treeCollision = treeBase->AddComponent<BoxColliderComponent>();
		treeCollision->SetExtents(glm::vec3(0.3f, 5, 0.3f));
		treeBase->name = "Tree";
		treeBase->transform.SetGlobalScale(glm::vec3(0.3f, 0.3f, 0.3f));
		treeBase->transform.SetLocalRotation(glm::quat(1, 90, 0, Random::RandomFloat(0, 360)));
		StaticMeshComponent* treeBaseSprite = treeBase->AddComponent<StaticMeshComponent>();
		if (treeBaseSprite != nullptr)
		{
			treeBaseSprite->SetMeshNew("Vegetation/Tree.fbx->Cylinder");
			treeBaseSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
			treeBaseSprite->SetShader(ResourceManager::GetShader("StandardShader"));
			treeBaseSprite->color = glm::vec4(0.43f, 0.286f, 0.035f, 1);
		}
		GameObject* treeLeaves = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
		treeLeaves->name = "Leaves";
		StaticMeshComponent* treeLeavesSprite = treeLeaves->AddComponent<StaticMeshComponent>();
		if (treeLeavesSprite != nullptr)
		{
			treeLeavesSprite->SetMeshNew("Vegetation/Tree.fbx->Icosphere");
			treeLeavesSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
			treeLeavesSprite->SetShader(ResourceManager::GetShader("StandardShader"));
			treeLeavesSprite->color = glm::vec4(0.278f, 0.5f, 0.039f, 1);
		}
		treeLeaves->transform.SetParent(treeBase);
		treeLeaves->transform.SetLocalPosition(glm::vec3(0, 0, 2));
	}

	//NavMesh agents
	for (size_t i = 0; i < 1; i++)
	{
		GameObject* agent = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(Random::RandomFloat(-worldSize / 2, worldSize / 2), 0.5f, Random::RandomFloat(-worldSize / 2, worldSize / 2)));
		agent->name = "NavMeshAgent";
		StaticMeshComponent* agentSprite = agent->AddComponent<StaticMeshComponent>();
		if (agentSprite != nullptr)
		{
			agentSprite->SetMeshNew("Cube.obj->Cube");
			agentSprite->SetTexture(ResourceManager::GetTexture("gizmoSelectTexture.jpg"));
			agentSprite->SetShader(ResourceManager::GetShader("StandardShader"));
		}
		agent->AddComponent<NavMeshAgentComponent>();
	}
}

GameManager::~GameManager()
{

}