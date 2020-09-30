#include "Component.h"
#include "../GameObject.h"
//#include <iostream>

Component::Component(GameObject* _parent) : name("Base Component")
{
	SetParent(_parent);
}

Component::~Component()
{
	//std::cout << "Default Component destructor\n";
	SetParent(nullptr);
}

Component::Component(const Component& comp) : name("BaseCompCopy"), parent(comp.parent), isActive(comp.isActive)
{
	//std::cout << "Base copy constuctor\n";
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

void Component::SetParent(GameObject* newParent)
{
	parent = newParent;
}

GameObject* const Component::GetParent() const
{
	return parent;
}

void Component::ShowOnGizmos()
{
}

void Component::ShowInInspectorBase()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Checkbox("Is Component Active", &isActive);
}