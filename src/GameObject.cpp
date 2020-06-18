#include "GameObject.h"
#include "ResourceManager.h"

#include <GLFW/glfw3.h>
#include <glm\ext\matrix_transform.hpp>

#include "Components/SpriteComponent.h"
#include "Components/MoveComponent.h"
#include "Components/BoxColliderComponent.h"

#include "Scene.h"

GameObject::GameObject() : name("NewGameObject"), position(glm::vec3(0,0,0))
{

}

GameObject::~GameObject()
{
	std::cout << "GO Destructor\n";
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
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	name = std::string(other.name);
	isActive = other.isActive;

	for (size_t i = 0; i < other.components.size(); i++)
	{
		components.push_back(other.components[i]->MakeCopy(this));
	}

	return *this;
}


void GameObject::ShowOnInspector(GameObject* selectedObj, Component* selectedComp)
{
	static int selectedComponentIndex = -1;

	//IsActive
	ImGui::Checkbox("Is Object Active", &isActive);

	//Name
	char* objName = name.data();
	ImGui::InputText("Name", objName, 64);
	name = std::string(objName);
	
	//Position
	ImGui::Text("Position");
	glm::vec3 pos = GetPosition();
	ImGui::InputFloat("Pos X", &pos.x, 0.5f, 1.0f);
	ImGui::InputFloat("Pos Y", &pos.y, 0.5f, 1.0f);
	ImGui::InputFloat("Pos Z", &pos.z, 0.5f, 1.0f);
	SetPosition(pos);

	//Rotation
	ImGui::Text("Rotation");
	glm::vec3 rotation = GetEulerAngles();
	ImGui::InputFloat("Rot X", &rotation.x, 1, 90);
	ImGui::InputFloat("Rot Y", &rotation.y, 1, 90);
	ImGui::InputFloat("Rot Z", &rotation.z, 1, 90);
	SetEulerAngles(rotation);

	//Scale
	ImGui::Text("Scale");
	glm::vec3 scale = GetScale();
	ImGui::InputFloat("Sca X", &scale.x, 0.1f, 1.0f);
	ImGui::InputFloat("Sca Y", &scale.y, 0.1f, 1.0f);
	ImGui::InputFloat("Sca Z", &scale.z, 0.1f, 1.0f);
	SetScale(scale);

	if (ImGui::Button("Add SpriteComponent"))
	{
		AddComponent<SpriteComponent>();
	}
	if (ImGui::Button("Add MoveComponent"))
	{
		AddComponent<MoveComponent>();
	}
	if (ImGui::Button("Add BoxColliderComponent"))
	{
		AddComponent<BoxColliderComponent>();
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
	position += offset;
}

void GameObject::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

glm::vec3 GameObject::GetPosition() const
{
	return position;
}

void GameObject::SetScale(const glm::vec3& newScale)
{
	scale = newScale;
}

glm::vec3 GameObject::GetScale() const
{
	return scale;
}

void GameObject::SetRotation(const glm::vec3& newRotation)
{
	rotation = newRotation;
}

glm::vec3 GameObject::GetRotation() const
{
	return rotation;
}

void GameObject::SetEulerAngles(const glm::vec3& newRotation)
{
	rotation = newRotation / 360.0f;
}

glm::vec3 GameObject::GetEulerAngles() const
{
	return rotation * 360.0f;
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