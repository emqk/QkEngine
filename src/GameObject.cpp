#include "GameObject.h"
#include "ResourceManager.h"

#include <GLFW/glfw3.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\vector_float3.hpp>

#include "Components/AnimatedSpriteComponent.h"
#include "Components/MoveComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/LightComponent.h"

#include "Scene.h"

GameObject::GameObject() : name("NewGameObject")
{
	transform.SetLocalPosition(glm::vec3(0, 0, 0));
}

GameObject::~GameObject()
{
	if (transform.GetParent() != nullptr)
	{
		transform.RemoveFromParent();
	}
	for (GameObject* child : transform.GetChilds())
	{
		child->transform.RemoveFromParent();
	}
}

GameObject::GameObject(const GameObject& a) 
{
	*this = a;
}


bool GameObject::operator==(const GameObject& other) const
{
	return this == &other;
}

GameObject& GameObject::operator=(const GameObject& other)
{
	name = std::string(other.name);
	isActive = other.isActive;
	
	for (size_t i = 0; i < other.components.size(); i++)
	{
		components.push_back(other.components[i]->MakeCopy(this));
	}

	transform = other.transform;
	transform = Transform(this);

	return *this;
}


void GameObject::ShowOnInspector(GameObject* selectedObj, Component* selectedComp)
{
	static int selectedComponentIndex = -1;

	//IsActive
	ImGui::Checkbox("Is Object Active", &isActive);
	if (ImGui::Button("Reset global position"))
		transform.SetGlobalPosition(glm::vec3(0, 0, 0));
	ImGui::SameLine();
	if (ImGui::Button("Reset global rotation"))
		transform.SetGlobalRotation(glm::quat(1, 0, 0, 0));
	ImGui::SameLine();
	if (ImGui::Button("Reset global scale"))
		transform.SetGlobalScale(glm::vec3(1, 1, 1));

	if (ImGui::Button("Remove from parent"))
		transform.RemoveFromParent();

	//Name
	char* objName = name.data();
	ImGui::InputText("Name", objName, 64);
	name = std::string(objName);

	//Position
	ImGui::Text("Position");
	glm::vec3 pos = transform.GetLocalPosition();
	ImGui::InputFloat("Pos X", &pos.x, 0.5f, 1.0f);
	ImGui::InputFloat("Pos Y", &pos.y, 0.5f, 1.0f);
	ImGui::InputFloat("Pos Z", &pos.z, 0.5f, 1.0f);
	transform.SetLocalPosition(pos);

	//Rotation
	ImGui::Text("Rotation");
	glm::quat localRotation = transform.GetLocalRotation();
	ImGui::InputFloat("Rot X", &localRotation.x, 1, 90);
	ImGui::InputFloat("Rot Y", &localRotation.y, 1, 90);
	ImGui::InputFloat("Rot Z", &localRotation.z, 1, 90);
	transform.SetLocalRotation(localRotation);

	//Scale
	ImGui::Text("Scale");
	glm::vec3 localScale = transform.GetLocalScale();
	ImGui::InputFloat("Sca X", &localScale.x, 0.1f, 1.0f);
	ImGui::InputFloat("Sca Y", &localScale.y, 0.1f, 1.0f);
	ImGui::InputFloat("Sca Z", &localScale.z, 0.1f, 1.0f);
	transform.SetLocalScale(localScale);

	if (ImGui::Button("Add StaticMeshComponent"))
	{
		AddComponent<StaticMeshComponent>();
	}
	if (ImGui::Button("Add AnimatedSpriteComponent"))
	{
		AddComponent<AnimatedSpriteComponent>();
	}
	if (ImGui::Button("Add MoveComponent"))
	{
		AddComponent<MoveComponent>();
	}
	if (ImGui::Button("Add BoxColliderComponent"))
	{
		AddComponent<BoxColliderComponent>();
	}
	if (ImGui::Button("Add LightComponent"))
	{
		AddComponent<LightComponent>();
	}
	if (ImGui::Button("Remove selected component"))
	{
		if (selectedComponentIndex >= 0 && selectedComponentIndex < GetComponentsCount())
		{
			RemoveComponent(selectedComponentIndex);
			selectedComponentIndex = -1;
		}
	}

	const std::vector<std::unique_ptr<Component>>* allGameObjectComps = GetAllComponents();
	size_t compSize = allGameObjectComps->size();
	std::vector<const char*> listbox_items;
	for (size_t i = 0; i < compSize; i++)
	{
		listbox_items.push_back((*allGameObjectComps)[i]->name.c_str());
	}

	ImGui::ListBox("Components", &selectedComponentIndex, listbox_items.data(), compSize, 8);

	if (selectedComponentIndex >= 0 && selectedComponentIndex < compSize)
	{
		(*allGameObjectComps)[selectedComponentIndex]->ShowInInspectorBase();
		(*allGameObjectComps)[selectedComponentIndex]->ShowOnInspector();
		selectedComp = (*allGameObjectComps)[selectedComponentIndex].get();
	}

	ImGui::Spacing();
	ImGui::Text("---------------------");
	ImGui::Spacing();

	Scene& currentScene = Scene::GetCurrentScene();

	if (ImGui::Button("Duplicate selected GameObject"))
	{
		currentScene.DuplicateGameObject(selectedObj);
		selectedObj = nullptr;
	}
	if (ImGui::Button("Destroy selected GameObject"))
	{
		currentScene.Destroy(selectedObj);
		selectedObj = nullptr;
	}
}

void GameObject::ShowOnGizmos()
{
	for (std::unique_ptr<Component>& comp : components)
	{
		comp->ShowOnGizmos();
	}
}

void GameObject::Update(const float& deltaTime)
{
	UpdateComponents(deltaTime);
}

void GameObject::LateUpdate(const float& deltaTime)
{
	LateUpdateComponents(deltaTime);
}

void GameObject::Move(const glm::vec3& offset)
{
	transform.Translate(offset);
}

void GameObject::SetActive(const bool& value)
{
	isActive = value;
}

bool GameObject::IsActive() const
{
	return isActive;
}

void GameObject::RemoveComponent(size_t index)
{
	components.erase(components.begin() + index);
}

const int& GameObject::GetComponentsCount() const
{
	return components.size();
}

const std::vector<std::unique_ptr<Component>>* const GameObject::GetAllComponents() const
{
	return &components;
}

void GameObject::UpdateComponents(const float& deltaTime)
{
	for (std::unique_ptr<Component>& comp : components)
	{
		if(comp->IsActiveSelf())
			comp->Update(deltaTime);
	}
}

void GameObject::LateUpdateComponents(const float& deltaTime)
{
	for (std::unique_ptr<Component>& comp : components)
	{
		if (comp->IsActiveSelf())
			comp->LateUpdate(deltaTime);
	}
}