#include "PlayerComponent.h"
#include "../GameObject.h"
#include "../ResourceManager.h"
#include "../InputManager.h"
#include "../Scene.h"
#include "../Camera.h"

#include <iostream>

PlayerComponent::PlayerComponent(GameObject* _parent) : Component(_parent)
{
	name = "PlayerComponent";
	spriteComponent = parent->AddComponent<SpriteComponent>();
	spriteComponent->SetMesh("Human/Human.obj");
	spriteComponent->SetTexture(ResourceManager::GetTexture("Human/Human_Walk_1.png"));
	spriteComponent->SetShader(ResourceManager::GetShader("StandardShader"));

	boxColliderComponent = parent->AddComponent<BoxColliderComponent>();
	boxColliderComponent->SetExtents(glm::vec3(0.5f, 1, 0.5f));

	moveComponent = parent->AddComponent<MoveComponent>();

	parent->SetScale(glm::vec3(1, 1, 1));
}

PlayerComponent::~PlayerComponent()
{
}

PlayerComponent::PlayerComponent(const PlayerComponent& comp) : Component(comp)
{
	std::cout << "PlayerComp copy constuctor\n";
	moveSpeed = comp.moveSpeed;
	currentGravity = comp.currentGravity;

	spriteComponent = parent->GetComponent<SpriteComponent>();
	boxColliderComponent = parent->GetComponent<BoxColliderComponent>();
	moveComponent = parent->GetComponent<MoveComponent>();
}

void PlayerComponent::Update(const float& deltaTime)
{
	if (moveComponent == nullptr)
	{
		std::cout << "MoveComp is nullptr!\n";
	}

	glm::vec3 moveVec(0,0,0);

	//Horizontal
	if (InputManager::GetKeyPressed(GLFW_KEY_D))
		moveVec += glm::vec3(1, 0, 0);
	if (InputManager::GetKeyPressed(GLFW_KEY_A))
		moveVec += glm::vec3(-1, 0, 0);
	//Verical
	if (InputManager::GetKeyPressed(GLFW_KEY_W))
		moveVec += glm::vec3(0, 0, -1);
	if (InputManager::GetKeyPressed(GLFW_KEY_S))
		moveVec += glm::vec3(0, 0, 1);


	if (glm::length(moveVec) > 0.0f)
		moveVec = glm::normalize(moveVec);
	
	//movement
	if (InputManager::GetKeyPressed(GLFW_KEY_LEFT_SHIFT))
		moveVec *= 2.0f;
	parent->Move(moveVec * moveSpeed * deltaTime);

	//gravity
	currentGravity += deltaTime * minGravity;
	if (currentGravity < minGravity)
		currentGravity = minGravity;

	if (InputManager::GetKeyPressed(GLFW_KEY_SPACE))
	{
		currentGravity += 25.0f * deltaTime;
	}

	parent->Move(glm::vec3(0, currentGravity, 0) * deltaTime);

	//Camera
	Camera* camera = &Scene::GetCurrentScene().GetCamera();
	glm::vec3 camPos = camera->GetPosition();
	glm::vec3 playerPos = parent->GetPosition();
	camera->SetPosition(glm::vec3(playerPos.x, 6, 15));
	camera->SetRotation(glm::vec3(-15, -90, 0));
}

void PlayerComponent::ShowOnInspector()
{
	ImGui::InputFloat("Move speed", &moveSpeed);
}

std::unique_ptr<Component> PlayerComponent::MakeCopy(GameObject* newParent) const
{
	std::cout << "Get copy of PlayerComp\n";
	std::unique_ptr<PlayerComponent> comp = std::make_unique<PlayerComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}