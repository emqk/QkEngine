#include "ResourceManager.h"
#include "ModelLoader.h"
#include <iostream>

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::textureMap;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourceManager::meshMap;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaderMap;


void ResourceManager::LoadTexture(const char* texturePath)
{
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	std::string fullPath = std::string( "../QkEngine/Resources/") + std::string(texturePath);
	std::cout << "Loading texture: " << fullPath.c_str() << "\n";
	texture->LoadTextureFromFile(fullPath.c_str());

	texture->name = texturePath;
	textureMap[texturePath] = std::move(texture);
}

Texture* ResourceManager::GetTexture(const char* texturePath)
{
	if(textureMap.find(texturePath) == textureMap.end())
		std::cout << "Can't find texture: " << texturePath << "\n";

	return textureMap[texturePath].get();
}

std::vector<std::string> ResourceManager::GetTexturesName()
{
	std::vector<std::string> names;
	for (const std::pair<const std::string, std::unique_ptr<Texture>>& p : textureMap)
	{
		names.push_back(p.first);
	}

	return std::move(names);
}

void ResourceManager::LoadMesh(const char* meshPath)
{
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	std::string fullPath = std::string("../QkEngine/Resources/") + std::string(meshPath);
	std::cout << "Loading mesh: " << fullPath.c_str() << "\n";

	MeshData data = ModelLoader::LoadObj(fullPath.c_str());
	mesh->SetVertices(data.vertices);
	mesh->SetIndices(data.indices);
	mesh->SetBounds(data.bounds);

 	std::cout << "Mesh loaded. Vert size: " << mesh->GetVertices().size() << "\n";

	mesh->name = meshPath;
	meshMap[meshPath] = std::move(mesh);
}

Mesh* ResourceManager::GetMesh(const char* meshPath)
{
	if (meshMap.find(meshPath) == meshMap.end())
		std::cout << "Can't find mesh: " << meshPath << "\n";

	return meshMap[meshPath].get();
}

std::vector<std::string> ResourceManager::GetMeshesName()
{
	std::vector<std::string> names;
	for (const std::pair<const std::string, std::unique_ptr<Mesh>>& p : meshMap)
	{
		names.push_back(p.first);
	}

	return std::move(names);
}

void ResourceManager::LoadShader(const char* shaderPath)
{
	std::string fullPath = std::string("../QkEngine/src/Shaders/") + std::string(shaderPath);
	std::string fullPathVS = fullPath + std::string(".vs");
	std::string fullPathFS = fullPath + std::string(".fs");
	
	std::cout << "Loading shader: " << fullPath.c_str() << "\n";
	std::unique_ptr<Shader> shader = std::make_unique<Shader>(fullPathVS.c_str(), fullPathFS.c_str());

	shader->name = shaderPath;
	shaderMap[shaderPath] = std::move(shader);
}

Shader* ResourceManager::GetShader(const char* shaderPath)
{
	if (shaderMap.find(shaderPath) == shaderMap.end())
		std::cout << "Can't find shader: " << shaderPath << "\n";

	return shaderMap[shaderPath].get();
}

std::vector<std::string> ResourceManager::GetShadersName()
{
	std::vector<std::string> names;
	for (const std::pair<const std::string, std::unique_ptr<Shader>>& p : shaderMap)
	{
		names.push_back(p.first);
	}

	return std::move(names);
}
