#include "StaticMeshComponent.h"
#include "../ResourceManager.h"
#include "../Renderer.h"

#include "../Editor.h"
#include "../Gizmos.h"

#include <iostream>


StaticMeshComponent::StaticMeshComponent(GameObject* _parent) : Component(_parent), color(1.0f, 1.0f, 1.0f, 1.0f)
{
	name = "StaticMeshComponent";
	Renderer::AddSpriteToDraw(this);
}

StaticMeshComponent::~StaticMeshComponent()
{
	Renderer::RemoveSpriteToDraw(this);
	mesh = nullptr;
	texture = nullptr;
	shader = nullptr;
}

StaticMeshComponent::StaticMeshComponent(const StaticMeshComponent& comp)
	: Component(comp), mesh(comp.mesh)
	, texture(comp.texture), specularTexture(comp.specularTexture)
	, shader(comp.shader)
	, color(comp.color), specular(comp.specular), shininess(comp.shininess)
{
	Renderer::AddSpriteToDraw(this);
}

void StaticMeshComponent::Update(const float& deltaTime)
{
}

void StaticMeshComponent::LateUpdate(const float& deltaTime)
{
}

void StaticMeshComponent::ShowOnGizmos()
{
	Gizmos::SetCurrentColor(glm::vec4(1,1,1,1));
	Gizmos::DrawCubeWireframe(parent->transform.GetGlobalPosition(), parent->transform.GetGlobalEulerAngles(), GetBounds().ExtentsHalf());
}

void StaticMeshComponent::ShowOnInspector()
{
	//Mesh
	std::string buttonStr = "No Mesh";
	if (mesh != nullptr)
		buttonStr = mesh->name;

	ImGui::Text("Mesh:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetMesh(texName.c_str()); };
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

	//SpecularTexture
	buttonStr = "No specular Texture";
	if (specularTexture != nullptr)
		buttonStr = specularTexture->name;

	ImGui::Text("Specular Texture:");
	ImGui::SameLine();
	if (ImGui::Button(buttonStr.c_str()))
	{
		std::function<void(std::string)> fun = [&](const std::string& texName) { this->SetSpecularTexture(texName.c_str()); };
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
	ImGui::ColorEdit4("diffuse", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::Text("Specular color:");
	ImGui::SameLine();
	ImGui::ColorEdit3("specular", (float*)&specular, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::DragFloat("Shininess", &shininess, 1.0f, 0.0f, 256.0f);
}

void StaticMeshComponent::SetMesh(const char* meshPath)
{
	std::cout << "Mesh set: " << meshPath << std::endl;
	mesh = ResourceManager::GetMesh(meshPath);
}

Mesh* StaticMeshComponent::GetMesh()
{
	return mesh;
}

void StaticMeshComponent::SetShader(Shader* _shader)
{
	shader = _shader;
}

Shader* StaticMeshComponent::GetShader()
{
	return shader;
}

Texture* StaticMeshComponent::GetTexture()
{
	return texture;
}

void StaticMeshComponent::SetSpecularTexture(const char* texPath)
{
	std::cout << "Texture set: " << texPath << std::endl;
	SetSpecularTexture(ResourceManager::GetTexture(texPath));
}

void StaticMeshComponent::SetSpecularTexture(Texture* tex)
{
	specularTexture = tex;
}

Texture* StaticMeshComponent::GetSpecularTexture()
{
	return specularTexture;
}

Bounds StaticMeshComponent::GetBounds() const
{
	Bounds scaledBounds;
	scaledBounds.SetExtents(mesh->GetBounds().Extents() * parent->transform.GetGlobalScale());
	return scaledBounds;
}

std::unique_ptr<Component> StaticMeshComponent::MakeCopy(GameObject* newParent) const
{
	std::unique_ptr<StaticMeshComponent> comp = std::make_unique<StaticMeshComponent>(*this);
	comp->parent = newParent;

	return std::move(comp);
}

void StaticMeshComponent::SetShader(const char* shaderPath)
{
	std::cout << "Shader set: " << shaderPath << std::endl;
	SetShader(ResourceManager::GetShader(shaderPath));
}

void StaticMeshComponent::SetTexture(const char* texPath)
{
	std::cout << "Texture set: " << texPath << std::endl;
	SetTexture(ResourceManager::GetTexture(texPath));
}

void StaticMeshComponent::SetTexture(Texture* tex)
{
	texture = tex;
}