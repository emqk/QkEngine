#include "PlayerComponent.h"
#include "../GameObject.h"
#include "../ResourceManager.h"
#include "../InputManager.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Gizmos.h"
#include "../Physics.h"
#include "../Transform.h"

#include <iostream>

PlayerComponent::PlayerComponent(GameObject* _parent) : Component(_parent)
{
	name = "PlayerComponent";

	boxColliderComponent = parent->AddComponent<BoxColliderComponent>();
	boxColliderComponent->SetExtents(glm::vec3(1, 1, 1));

	moveComponent = parent->AddComponent<MoveComponent>();

	parent->SetLocalRotation(glm::vec3(1, 1, 1));
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
	cameraSensitivity = comp.cameraSensitivity;

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
	std::vector<BoxColliderComponent*> collidingWith = Physics::BoxCast(parent->GetTransform().GetLocalPosition() + groundDetectorOffset, groundDetectorScale / 2.0f);
	for (const BoxColliderComponent* col : collidingWith)
	{
		if (col->GetParent()->GetComponent<PlayerComponent>() == nullptr)
		{
			isGrounded = true;
			//std::cout << "Colliding with: " << col->GetParent()->name << std::endl;
		}
	}

	glm::vec3 moveVec(0,0,0);

	glm::vec3 localRight = parent->GetTransform().GetRight();
	glm::vec3 localForward = parent->GetTransform().GetForward();

	//Horizontal
	if (InputManager::GetKey(GLFW_KEY_D))
		moveVec += localRight;
	if (InputManager::GetKey(GLFW_KEY_A))
		moveVec += -localRight;
	//Verical
	if (InputManager::GetKey(GLFW_KEY_W))
		moveVec += localForward;
	if (InputManager::GetKey(GLFW_KEY_S))
		moveVec += -localForward;


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
	glm::vec3 camPos = camera->GetLocalPosition();
	glm::vec3 playerPos = parent->GetTransform().GetLocalPosition();

	glm::vec2 currMousePos = Scene::GetCurrentScene().GetMousePos();
	glm::vec2 mouseMove = currMousePos - previousMousePos;

	targetRotY += mouseMove.x * cameraSensitivity * deltaTime;
	targetRotX += -mouseMove.y * cameraSensitivity * deltaTime;
	targetRotX = glm::clamp(targetRotX, -80.0f, 80.0f);

	parent->SetLocalRotation(Transform::ToQuaternion(glm::vec3(parent->GetTransform().GetLocalRotation().x, targetRotY, parent->GetTransform().GetLocalRotation().z)));
	camera->SetLocalPosition(parent->GetTransform().GetLocalPosition());
	camera->SetLocalRotation(Transform::ToQuaternion(glm::vec3(targetRotX, parent->GetTransform().GetLocalRotation().y, parent->GetTransform().GetLocalRotation().z)));

	previousMousePos = currMousePos;
}

void PlayerComponent::ShowOnInspector()
{
	ImGui::InputFloat("Move speed", &moveSpeed);
	ImGui::InputFloat("Camera sensitivity", &cameraSensitivity);
}

void PlayerComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(Gizmos::defaultColor);
	Gizmos::DrawCubeWireframe(parent->GetTransform().GetLocalPosition() + groundDetectorOffset, glm::vec3(0, 0, 0), groundDetectorScale);
}

std::unique_ptr<Component> PlayerComponent::MakeCopy(GameObject* newParent) const
{
	std::cout << "Get copy of PlayerComp\n";
	std::unique_ptr<PlayerComponent> comp = std::make_unique<PlayerComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}