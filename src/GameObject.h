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

	void Move(const glm::vec3& offset);


	const Transform& GetTransform() const;

	glm::vec3 GetGlobalPosition() const;
	glm::quat GetGlobalRotation() const;
	glm::vec3 GetGlobalScale() const;

	void SetLocalPosition(const glm::vec3& pos);
	void SetLocalRotation(const glm::quat& rot);
	void SetLocalScale(const glm::vec3& scale);
	glm::vec3 GetLocalPosition() const;
	glm::quat GetLocalRotation() const;
	glm::vec3 GetLocalScale() const;

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
	void SetParent(GameObject* newParent);
	const GameObject const* GetParent() const;

	std::string name;

protected:
	void UpdateComponents(const float& deltaTime);
	void LateUpdateComponents(const float& deltaTime);

	static void RemoveFromParent(GameObject* child);

private:
	bool isActive = true;
	GameObject* parent = nullptr;
	Transform transform;

	std::vector<GameObject*> childs;
	std::vector<std::unique_ptr<Component>> components;
};