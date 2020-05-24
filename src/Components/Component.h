#pragma once
#include <string>
#include <memory>
#include "../imgui/imgui.h"

class GameObject;

class Component
{
public:
	Component(GameObject* _parent);
	virtual ~Component();

	Component(const Component& comp);
	Component& operator=(const Component& comp) = delete;

	virtual void Update(const float& deltaTime) = 0;
	void ShowInInspectorBase();
	virtual void ShowOnInspector() = 0;

	bool IsParentActive() const;
	void SetActiveSelf(const bool& value);
	bool IsActive() const;
	bool IsActiveSelf() const;

	GameObject* const GetParent() const;

	std::string name;
	virtual std::unique_ptr<Component> MakeCopy(GameObject* newParent) const = 0;

protected:
	bool isActive = true;
	GameObject* parent;
};