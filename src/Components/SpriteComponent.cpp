#include "SpriteComponent.h"
#include "../ResourceManager.h"
#include "../Renderer.h"

#include "../Editor.h"
#include "../Gizmos.h"

#include <iostream>


SpriteComponent::SpriteComponent(GameObject* _parent) : Component(_parent), color(1.0f, 1.0f, 1.0f, 1.0f)
{
	name = "SpriteComponent";
	Renderer::AddSpriteToDraw(this);
}

SpriteComponent::~SpriteComponent()
{
	Renderer::RemoveSpriteToDraw(this);
	meshNew = nullptr;
	texture = nullptr;
	shader = nullptr;
}

SpriteComponent::SpriteComponent(const SpriteComponent& comp) : Component(comp), meshNew(comp.meshNew), texture(comp.texture), shader(comp.shader), color(comp.color)
{
	Renderer::AddSpriteToDraw(this);
}

void SpriteComponent::Update(const float& deltaTime)
{
}

void SpriteComponent::LateUpdate(const float& deltaTime)
{
}

void SpriteComponent::ShowOnInspector()
{
	//MeshNEW
	std::string buttonStr = "No Mesh";
	if (meshNew != nullptr)
		buttonStr = meshNew->name;

	ImGui::Text("Mesh:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetMeshNew(texName.c_str()); };
		Editor::ShowSelectAssetWindow(AssetWindowType::MeshesNew, fun);
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

	ImGui::Text("Base color:");
	ImGui::SameLine();
	ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
}

void SpriteComponent::SetMeshNew(const char* meshPath)
{
	std::cout << "Mesh set: " << meshPath << std::endl;
	meshNew = ResourceManager::GetMeshNew(meshPath);
}

Mesh* SpriteComponent::GetMeshNew()
{
	return meshNew;
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
	std::cout << "Shader set: " << shaderPath << std::endl;
	SetShader(ResourceManager::GetShader(shaderPath));
}

void SpriteComponent::SetTexture(const char* texPath)
{
	std::cout << "Texture set: " << texPath << std::endl;
	SetTexture(ResourceManager::GetTexture(texPath));
}

void SpriteComponent::SetTexture(Texture* tex)
{
	texture = tex;
}