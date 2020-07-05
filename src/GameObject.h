#pragma once
#include <glm\ext\vector_float3.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Components/Component.h"
#include "Transform.h"

#include <vector>
#include <iostream>
#include <memory>

class GameObject
{
public:
	GameObject();
	~GameObject();

	GameObject(const GameObject& a);
	bool operator==(const GameObject& other) const;
	GameObject& operator=(const GameObject& other);

	void ShowOnInspector(GameObject* selectedObj, Component* selectedComp);
	virtual void ShowOnGizmos();
	virtual void Update(const float& deltaTime);
	virtual void LateUpdate(const float& deltaTime);

	void Move(const glm::vec3& offset);
	void SetLocalPosition(const glm::vec3& pos);
	glm::vec3 GetLocalPosition() const;

	void SetLocalScale(const glm::vec3& newScale);
	glm::vec3 GetLocalScale() const;
		
	void SetLocalRotation(const glm::quat& newRotation);
	glm::quat GetLocalRotation() const;
	
	const Transform& GetTransform() const;

	void SetActive(const bool& value);
	bool IsActive() const;

	template<typename T>
	T* AddComponent()
	{
		std::unique_ptr<T> newComp = std::make_unique<T>(this);
		components.push_back(std::move(newComp));

		return reinterpret_cast<T*>(components.back().get());
	}

	template<typename T>
	T* const GetComponent()
	{
		for (const std::unique_ptr<Component>& comp : components)
		{
			T* converted = dynamic_cast<T*>(comp.get());
			if (converted != nullptr)
			{
				return converted;
			}
		}

		return nullptr;
	}

	void RemoveComponent(size_t index);

	const int& GetComponentsCount() const;
	const std::vector<std::unique_ptr<Component>>* const GetAllComponents() const;
	const std::vector<GameObject*>& GetChilds() const;


	void ForgetParentAndChilds();
	void AddChild(GameObject* child);
	const GameObject const* GetParent() const;

	std::string name;

protected:
	void UpdateComponents(const float& deltaTime);
	void LateUpdateComponents(const float& deltaTime);

	static void RemoveFromParent(GameObject* child);

private:
	Transform transform;
	bool isActive = true;
	GameObject* parent = nullptr;

	std::vector<GameObject*> childs;
	std::vector<std::unique_ptr<Component>> components;
};