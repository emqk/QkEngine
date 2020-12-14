#include "PlayerComponent.h"
#include "../GameObject.h"
#include "../ResourceManager.h"
#include "../Audio/AudioManager.h"
#include "../InputManager.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Gizmos.h"
#include "../Physics/Physics.h"
#include "../Transform.h"

#include <iostream>

PlayerComponent::PlayerComponent(GameObject* _parent) : Component(_parent)
{
	name = "PlayerComponent";

	//if (!boxColliderComponent)
	//{
	//	boxColliderComponent = parent->AddComponent<BoxColliderComponent>();
	//	boxColliderComponent->SetExtents(glm::vec3(0.5f, 1, 0.5f));
	//}
	//if (!moveComponent)
	//{
	//	moveComponent = parent->AddComponent<MoveComponent>();
	//}
	//if (!animatedSpriteComponent)
	//{
	//	animatedSpriteComponent = parent->AddComponent<AnimatedSpriteComponent>();
	//	animatedSpriteComponent->SetMesh("Human/Human.obj->Plane");
	//	animatedSpriteComponent->SetTexture(ResourceManager::GetTexture("Adventurer/adventurer-idle-00.png"));
	//	animatedSpriteComponent->SetShader(ResourceManager::GetShader("StandardShader"));
	//	animatedSpriteComponent->SetCurrentAnimation(ResourceManager::GetSpriteAnimation("Idle"));
	//}
}

PlayerComponent::~PlayerComponent()
{
}

PlayerComponent::PlayerComponent(const PlayerComponent& comp) : Component(comp)
{
	moveSpeed = comp.moveSpeed;
	currentGravity = comp.currentGravity;
	groundDetectorOffset = comp.groundDetectorOffset;
	groundDetectorScale = comp.groundDetectorScale;

	boxColliderComponent = parent->GetComponent<BoxColliderComponent>();
	moveComponent = parent->GetComponent<MoveComponent>();
	animatedSpriteComponent = parent->GetComponent<AnimatedSpriteComponent>();
}

void PlayerComponent::Start()
{
	boxColliderComponent = parent->GetComponent<BoxColliderComponent>();
	moveComponent = parent->GetComponent<MoveComponent>();
	animatedSpriteComponent = parent->GetComponent<AnimatedSpriteComponent>();

	playerWidget = Scene::GetCurrentScene().InstantiateWidget<Widget>(glm::vec2(0.9f, 0.9f), glm::vec2(0.1f, 0.1f));
	playerButtonWidget = Scene::GetCurrentScene().InstantiateWidget<ButtonWidget>(glm::vec2(0.5f, 0.7f), glm::vec2(0.2f, 0.1f));
	playerButtonWidget->SetOnClick([&]() { moveSpeed += 0.5f; std::cout << "[PLAYER_COMPONENT LAMBDA] Speed up player!\n"; });
}

void PlayerComponent::Update(const float& deltaTime)
{
	if (moveComponent == nullptr)
	{
		std::cout << "MoveComp is nullptr!\n";
	}

	isGrounded = false;
	std::vector<BoxColliderComponent*> collidingWith = Physics::BoxCast(parent->transform.GetLocalPosition() + groundDetectorOffset, groundDetectorScale / 2.0f);
	for (const BoxColliderComponent* col : collidingWith)
	{
		if (col->GetParent()->GetComponent<PlayerComponent>() == nullptr)
		{
			isGrounded = true;
			//std::cout << "Colliding with: " << col->GetParent()->name << std::endl;
		}
	}

	glm::vec3 moveVec(0,0,0);

	//Horizontal
	if (InputManager::GetKey(GLFW_KEY_D))
		moveVec += parent->transform.GetRight();
		//moveVec += glm::vec3(1, 0, 0);
	if (InputManager::GetKey(GLFW_KEY_A))
		moveVec += -parent->transform.GetRight();
	//Verical
	if (InputManager::GetKey(GLFW_KEY_W))
		moveVec += parent->transform.GetForward();
	if (InputManager::GetKey(GLFW_KEY_S))
		moveVec += -parent->transform.GetForward();

	float moveVecLength = glm::length(moveVec);
	if (moveVecLength > 0.0f)
	{
		moveVec = glm::normalize(moveVec);
		glm::vec3 currScale = parent->transform.GetLocalScale();
		if (moveVec.x >= 0.0f)
			parent->transform.SetGlobalScale(glm::vec3(abs(currScale.x), currScale.y, currScale.z));
		else if (moveVec.x < 0.0f)
			parent->transform.SetGlobalScale(glm::vec3(-abs(currScale.x), currScale.y, currScale.z));
	}
	
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
		AudioManager::PlayAudioClip(ResourceManager::GetAudioClip("Jump.wav"));

		//Spawn particle
		Texture* tex = ResourceManager::GetTexture("Dust.png");
		Shader* shader = ResourceManager::GetShader("StandardShader");
		Mesh* mesh = ResourceManager::GetMesh("Human/Human.obj->Plane");
		ParticleManager::SpawnEmitter(std::make_unique<ParticleSystem>
			(GetParent()->transform.GetGlobalPosition() + groundDetectorOffset , glm::vec3(0.5f, 0.5f, 0.5f), tex, mesh, shader, glm::vec4(1, 1, 1, 1), 10));
	}

	parent->Move(glm::vec3(0, currentGravity, 0) * deltaTime);
	ControlAnimations(moveVec);
	if(animatedSpriteComponent)
		playerWidget->SetTexture(animatedSpriteComponent->GetTexture());
}

void PlayerComponent::LateUpdate(const float& deltaTime)
{
	//Camera
	Camera* camera = &Scene::GetCurrentScene().GetCamera();
	glm::vec3 camPos = camera->GetLocalPosition();
	glm::vec3 playerPos = parent->transform.GetLocalPosition();

	camera->SetLocalPosition(parent->transform.GetGlobalPosition() + glm::vec3(0, 7, 10));
	glm::quat targetRot = glm::lookAt(camera->GetLocalPosition(), parent->transform.GetGlobalPosition(), glm::vec3(0, 1, 0));
	camera->SetLocalRotation(Transform::ConvertQuaternionToQuaternionEulerAngles(targetRot));
}

void PlayerComponent::ShowOnInspector()
{
	ImGui::InputFloat("Move speed", &moveSpeed);
}

void PlayerComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(Gizmos::defaultColor);
	Gizmos::DrawCubeWireframe(parent->transform.GetLocalPosition() + groundDetectorOffset, glm::vec3(0, 0, 0), groundDetectorScale);
}

std::unique_ptr<Component> PlayerComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<PlayerComponent> comp = std::make_unique<PlayerComponent>(*this);
	comp->parent = newParent;

	comp->boxColliderComponent = comp->parent->GetComponent<BoxColliderComponent>();
	comp->moveComponent = comp->parent->GetComponent<MoveComponent>();
	comp->animatedSpriteComponent = comp->parent->GetComponent<AnimatedSpriteComponent>();

	return std::move(comp);
}

void PlayerComponent::ControlAnimations(const glm::vec3& moveVec)
{
	if (!animatedSpriteComponent)
		return;

	float moveVecLength = glm::length(moveVec);
	if (moveVecLength > 0.0f)
	{
		SpriteAnimation* anim = ResourceManager::GetSpriteAnimation("Run");
		animatedSpriteComponent->SetCurrentAnimation(anim);
	}
	else
	{
		SpriteAnimation* anim = ResourceManager::GetSpriteAnimation("Idle");
		animatedSpriteComponent->SetCurrentAnimation(anim);
	}

	if (!isGrounded)
	{
		SpriteAnimation* anim = ResourceManager::GetSpriteAnimation("Jump");
		animatedSpriteComponent->SetCurrentAnimation(anim);
	}
}
