#include "PlayerComponent.h"
#include "../GameObject.h"
#include "../ResourceManager.h"
#include "../InputManager.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Gizmos.h"
#include "../Physics.h"

#include <iostream>

PlayerComponent::PlayerComponent(GameObject* _parent) : Component(_parent)
{
	name = "PlayerComponent";
	spriteComponent = parent->AddComponent<SpriteComponent>();
	spriteComponent->SetMeshNew("Cube.obj->Cube");
	spriteComponent->SetTexture(ResourceManager::GetTexture("Cube.jpg"));
	spriteComponent->SetShader(ResourceManager::GetShader("StandardShader"));

	boxColliderComponent = parent->AddComponent<BoxColliderComponent>();
	boxColliderComponent->SetExtents(glm::vec3(1, 1, 1));

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
	groundDetectorOffset = comp.groundDetectorOffset;
	groundDetectorScale = comp.groundDetectorScale;

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

	bool isGrounded = false;
	std::vector<BoxColliderComponent*> collidingWith = Physics::BoxCast(parent->GetPosition() + groundDetectorOffset, groundDetectorScale / 2.0f);
	for (const BoxColliderComponent* col : collidingWith)
	{
		if (col->GetParent()->GetComponent<PlayerComponent>() == nullptr)
		{
			isGrounded = true;
			std::cout << "Colliding with: " << col->GetParent()->name << std::endl;
		}
	}

	glm::vec3 moveVec(0,0,0);

	//Horizontal
	if (InputManager::GetKey(GLFW_KEY_D))
		moveVec += glm::vec3(1, 0, 0);
	if (InputManager::GetKey(GLFW_KEY_A))
		moveVec += glm::vec3(-1, 0, 0);
	//Verical
	if (InputManager::GetKey(GLFW_KEY_W))
		moveVec += glm::vec3(0, 0, -1);
	if (InputManager::GetKey(GLFW_KEY_S))
		moveVec += glm::vec3(0, 0, 1);


	if (glm::length(moveVec) > 0.0f)
		moveVec = glm::normalize(moveVec);
	
	//movement
	if (InputManager::GetKey(GLFW_KEY_LEFT_SHIFT))
		moveVec *= 2.0f;
	parent->Move(moveVec * moveSpeed * deltaTime);

	//gravity
	if (isGrounded)
	{
		currentGravity = 0;
	}
	else
	{
		currentGravity += deltaTime * minGravity;
		if (currentGravity < minGravity)
			currentGravity = minGravity;
	}

	if (InputManager::GetKeyDown(GLFW_KEY_SPACE) && isGrounded)
	{
		currentGravity += 6;
	}

	parent->Move(glm::vec3(0, currentGravity, 0) * deltaTime);
}

void PlayerComponent::LateUpdate(const float& deltaTime)
{
	//Camera
	Camera* camera = &Scene::GetCurrentScene().GetCamera();
	glm::vec3 camPos = camera->GetPosition();
	glm::vec3 playerPos = parent->GetPosition();
	//camera->SetPosition(glm::vec3(playerPos.x, playerPos.y + 10, playerPos.z + 10));
	//camera->SetRotation(glm::vec3(-45, -90, 0));
	camera->SetPosition(parent->GetPosition());
	camera->SetRotation(glm::vec3(0,0,0));
}

void PlayerComponent::ShowOnInspector()
{
	ImGui::InputFloat("Move speed", &moveSpeed);
}

void PlayerComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(Gizmos::defaultColor);
	Gizmos::DrawCubeWireframe(parent->GetPosition() + groundDetectorOffset, glm::vec3(0, 0, 0), groundDetectorScale);
}

std::unique_ptr<Component> PlayerComponent::MakeCopy(GameObject* newParent) const
{
	std::cout << "Get copy of PlayerComp\n";
	std::unique_ptr<PlayerComponent> comp = std::make_unique<PlayerComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}