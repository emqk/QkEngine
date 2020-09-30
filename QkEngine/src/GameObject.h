#pragma once
#include <glm\ext\vector_float3.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Components/Component.h"
#include "Transform.h"

#include <vector>
#include <iostream>
#include <memory>
#include <functional>

class GameObject
{
	friend Transform;

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
	void ShowComponentsToAdd(const char* componentName);

	void Move(const glm::vec3& offset);

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
	T* GetComponent()
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


	std::string name;
	Transform transform{ this };

protected:
	void UpdateComponents(const float& deltaTime);
	void LateUpdateComponents(const float& deltaTime);


private:
	bool isActive = true;
	std::vector<std::unique_ptr<Component>> components;
};