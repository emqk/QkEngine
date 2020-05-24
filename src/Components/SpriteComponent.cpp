#include "SpriteComponent.h"
#include "../ResourceManager.h"
#include "../Renderer.h"

#include "../Editor.h"

#include <iostream>


SpriteComponent::SpriteComponent(GameObject* _parent) : Component(_parent)
{
	name = "SpriteComponent";
	Renderer::AddSpriteToDraw(this);
}

SpriteComponent::~SpriteComponent()
{
	Renderer::RemoveSpriteToDraw(this);
	mesh = nullptr;
	texture = nullptr;
	shader = nullptr;

	std::cout << "SpriteComponent destructor\n";
}

SpriteComponent::SpriteComponent(const SpriteComponent& comp) : Component(comp), mesh(comp.mesh), texture(comp.texture), shader(comp.shader)
{
	std::cout << "SpriteComp copy constuctor\n";

	Renderer::AddSpriteToDraw(this);
}

void SpriteComponent::Update(const float& deltaTime)
{
}

void SpriteComponent::ShowOnInspector()
{
	ImGui::Text("Properties");

	//Mesh
	std::string buttonStr = "No Mesh";
	if (mesh != nullptr)
		buttonStr = mesh->name;

	ImGui::Text("Mesh:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetMesh(texName.c_str()); };
		Editor::ShowSelectAssetWindow(AssetWindowType::Meshes, fun);
	}

	//Texture
	buttonStr = "No Texture";
	if (texture != nullptr)
		buttonStr = texture->name;

	ImGui::Text("Texture:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetTexture(texName.c_str()); };
		Editor::ShowSelectAssetWindow(AssetWindowType::Textures, fun);
	}

	//Shader
	buttonStr = "No Shader";
	if (shader != nullptr)
		buttonStr = shader->name;

	ImGui::Text("Shader:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetShader(texName.c_str()); };
		Editor::ShowSelectAssetWindow(AssetWindowType::Shaders, fun);
	}
}

void SpriteComponent::SetMesh(const char* meshPath)
{
	mesh = ResourceManager::GetMesh(meshPath);
}

Mesh* SpriteComponent::GetMesh()
{
	return mesh;
}

void SpriteComponent::SetShader(Shader* _shader)
{
	shader = _shader;
}

Shader* SpriteComponent::GetShader()
{
	return shader;
}

const Texture* SpriteComponent::GetTexture()
{
	return texture;
}

std::unique_ptr<Component> SpriteComponent::MakeCopy(GameObject* newParent) const
{
	std::cout << "Get copy of spriteComp\n";
	std::unique_ptr<SpriteComponent> comp = std::make_unique<SpriteComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void SpriteComponent::SetShader(const char* shaderPath)
{
	SetShader(ResourceManager::GetShader(shaderPath));
}

void SpriteComponent::SetTexture(const char* texPath)
{
	SetTexture(ResourceManager::GetTexture(texPath));
}

void SpriteComponent::SetTexture(Texture* tex)
{
	texture = tex;
}