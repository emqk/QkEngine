#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../imgui/imgui.h"

class GameObject;

class Component
{
public:
	Component(GameObject* _parent);
	virtual ~Component();

	Component(const Component& comp);
	Component& operator=(const Component& comp) = delete;

	virtual void Start() {};
	virtual void Update(const float& deltaTime) {};
	virtual void LateUpdate(const float& deltaTime) {};

	virtual void ShowOnGizmos();
	void ShowInInspectorBase();
	virtual void ShowOnInspector() {};

	bool IsParentActive() const;
	void SetActiveSelf(const bool& value);
	bool IsActive() const;
	bool IsActiveSelf() const;

	GameObject* const GetParent() const;

	std::string name;
	virtual std::unique_ptr<Component> MakeCopy(GameObject* newParent) const = 0;

protected:
	void SetParent(GameObject* newParent);

	bool isActive = true;
	GameObject* parent;
};