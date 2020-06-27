#include "ResourceManager.h"
#include <iostream>

class MeshData;

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::textureMap;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaderMap;

std::unordered_map<std::string, std::unique_ptr<Model>> ResourceManager::modelMap;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourceManager::meshNewMap;

//struct Vertex {
//	// position
//	glm::vec3 Position;
//	// normal
//	glm::vec3 Normal;
//	// texCoords
//	glm::vec2 TexCoords;
//	// tangent
//	glm::vec3 Tangent;
//	// bitangent
//	glm::vec3 Bitangent;
//};

void ResourceManager::LoadTexture(const char* texturePath)
{
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	std::string fullPath = std::string( "../QkEngine/Resources/") + std::string(texturePath);
	std::cout << "Loading texture: " << fullPath.c_str() << "\n";
	bool loaded = texture->LoadTextureFromFile(fullPath.c_str());

	texture->name = texturePath;
	textureMap[texturePath] = std::move(texture);
}

Texture* ResourceManager::GetTexture(const char* texturePath)
{
    if (textureMap.find(texturePath) == textureMap.end())
    {
		std::cout << "Can't find texture: " << texturePath << "\n";
		std::cout << "I have: "<< "\n";
        for (std::pair<std::string, const std::unique_ptr<Texture>&> p : textureMap)
        {
            std::cout << "element: " << p.first << "\n";
        }
    }

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

void ResourceManager::LoadModel(const char* meshPath)
{
	std::cout << "Loading model: " << meshPath << "\n";
	std::string fullPath = std::string("../QkEngine/Resources/") + std::string(meshPath);
    std::unique_ptr<Model> model = std::make_unique<Model>(fullPath, meshPath);

    modelMap[meshPath] = std::move(model);
}

Model* ResourceManager::GetModel(const char* meshPath)
{
    if (modelMap.find(meshPath) == modelMap.end())
        std::cout << "Can't find model: " << meshPath << "\n";

    return modelMap[meshPath].get();
}

void ResourceManager::LoadMesh(Model* model)
{
    for (size_t i = 0; i < model->meshes.size(); i++)
    {
        Mesh* currMesh = &model->meshes[i];
        std::string id = model->GetShortDirectory() + std::string("->") + currMesh->name;
        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(currMesh->GetVertices(), currMesh->GetIndices(), currMesh->GetBounds(), id);
		std::cout << "\tLoaded mesh: " << currMesh->name << "\n";

        meshNewMap[id] = std::move(mesh);
    }
}

Mesh* ResourceManager::GetMeshNew(const char* shortMeshPath)
{
    if (meshNewMap.find(shortMeshPath) == meshNewMap.end())
        std::cout << "Can't find (shortPath) meshNew: " << shortMeshPath << "\n";

    return meshNewMap[shortMeshPath].get();
}

std::vector<std::string> ResourceManager::GetMeshesNewName()
{
    std::vector<std::string> names;
    for (const std::pair<const std::string, std::unique_ptr<Mesh>>& p : meshNewMap)
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