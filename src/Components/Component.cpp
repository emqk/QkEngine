#include "Component.h"
#include "../GameObject.h"
#include <iostream>

Component::Component(GameObject* _parent) : name("Base Component")
{
	parent = _parent;
}

Component::~Component()
{
	std::cout << "Default Component destructor\n";
	parent = nullptr;
}

Component::Component(const Component& comp) : name("BaseCompCopy"), parent(comp.parent), isActive(comp.isActive)
{
	std::cout << "Base copy constuctor\n";
	name = comp.name;
	parent = comp.parent;
	isActive = comp.isActive;
}

bool Component::IsParentActive() const
{
	return parent->IsActive();
}

void Component::SetActiveSelf(const bool& value)
{
	isActive = value;
}

bool Component::IsActive() const
{
	return isActive && IsParentActive();
}

bool Component::IsActiveSelf() const
{
	return isActive;
}

GameObject* const Component::GetParent() const
{
	return parent;
}

void Component::ShowInInspectorBase()
{
	ImGui::Checkbox("Is Component Active", &isActive);
}