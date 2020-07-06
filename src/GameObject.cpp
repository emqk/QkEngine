#include "GameObject.h"
#include "ResourceManager.h"

#include <GLFW/glfw3.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\vector_float3.hpp>

#include "Components/SpriteComponent.h"
#include "Components/MoveComponent.h"
#include "Components/BoxColliderComponent.h"

#include "Scene.h"

GameObject::GameObject() : name("NewGameObject")
{
	transform.SetLocalPosition(glm::vec3(0, 0, 0));
	transform.SetRoot(this);
}

GameObject::~GameObject()
{
	std::cout << "GO Destructor\n";

	if (GetParent() != nullptr)
	{
		RemoveFromParent(this);
	}
	for (GameObject* child : childs)
	{
		RemoveFromParent(child);
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
	transform = other.transform;
	transform.SetRoot(this);
	name = std::string(other.name);
	isActive = other.isActive;
	childs = other.childs;
	SetParent(other.parent);

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
	transform.Translate(offset);
}

const Transform& GameObject::GetTransform() const
{
	return transform;
}

glm::vec3 GameObject::GetGlobalPosition() const
{
	return glm::vec3();
}

glm::quat GameObject::GetGlobalRotation() const
{
	return glm::quat();
}

glm::vec3 GameObject::GetGlobalScale() const
{
	return glm::vec3();
}

void GameObject::SetLocalPosition(const glm::vec3& pos)
{
	transform.SetLocalPosition(pos);
}

void GameObject::SetLocalRotation(const glm::quat& rot)
{
	transform.SetLocalRotation(rot);
}

void GameObject::SetLocalScale(const glm::vec3& scale)
{
	transform.SetLocalScale(scale);
}

glm::vec3 GameObject::GetLocalPosition() const
{
	return transform.GetGlobalPosition();
}

glm::quat GameObject::GetLocalRotation() const
{
	return transform.GetGlobalRotation();
}

glm::vec3 GameObject::GetLocalScale() const
{
	return transform.GetGlobalScale();
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

const std::vector<GameObject*>& GameObject::GetChilds() const
{
	return childs;
}


void GameObject::ForgetParentAndChilds()
{
	SetParent(nullptr);
	childs.clear();
}

void GameObject::AddChild(GameObject* child)
{
	if (child->GetParent() != nullptr)
	{
		if (child->GetParent() == this)
		{
			std::cout<<("Can't add child to the same parent as it's current parent! [RETURN]");
			return;
		}

		//Remove child from it's current parent
		RemoveFromParent(child);
	}

	childs.push_back(child);
	child->SetParent(this);
}

void GameObject::SetParent(GameObject* newParent)
{
	parent = newParent;



	//Only to refresh Transform. Need to remove it later
		transform.SetRoot(this);
	///////////////////////////



}

void GameObject::RemoveFromParent(GameObject* child)
{
	if (child->parent != nullptr)
	{
		std::cout << child->parent->name;
		std::vector<GameObject*>::iterator it = std::find(child->parent->childs.begin(), child->parent->childs.end(), child);
		if (it != child->parent->childs.end())
			child->parent->childs.erase(it);

		child->SetParent(nullptr);
	}
	else
	{
		std::cout << "Can't remove from parent - Parent is null!\n";
	}
}


const GameObject const* GameObject::GetParent() const
{
	return parent;
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