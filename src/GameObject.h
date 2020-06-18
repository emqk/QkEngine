#pragma once
#include <glm\ext\vector_float3.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Components/Component.h"

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
	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;

	void SetScale(const glm::vec3& newScale);
	glm::vec3 GetScale() const;
		
	void SetRotation(const glm::vec3& newRotation);
	glm::vec3 GetRotation() const;
	
	void SetEulerAngles(const glm::vec3& newRotation);
	glm::vec3 GetEulerAngles() const;

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

	std::string name;

protected:
	void UpdateComponents(const float& deltaTime);
	void LateUpdateComponents(const float& deltaTime);


private:
	glm::vec3 position;
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	bool isActive = true;

	std::vector<std::unique_ptr<Component>> components;
};