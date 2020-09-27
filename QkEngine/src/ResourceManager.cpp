#include "ResourceManager.h"
#include "Audio/AudioManager.h"
#include <iostream>

class MeshData;

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::textureMap;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaderMap;

std::unordered_map<std::string, std::unique_ptr<SpriteAnimation>> ResourceManager::spriteAnimationMap;

std::unordered_map<std::string, std::unique_ptr<Model>> ResourceManager::modelMap;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourceManager::meshNewMap;

std::unordered_map<std::string, irrklang::ISoundSource*> ResourceManager::audioClipMap;


void ResourceManager::LoadSpriteAnimation(std::vector<Texture*> _textures, const char* name)
{
	spriteAnimationMap[name] = std::move(std::make_unique<SpriteAnimation>(_textures));
	std::cout << "Loaded animation: " << name << "\n";
}

SpriteAnimation* ResourceManager::GetSpriteAnimation(const char* name)
{
	if (spriteAnimationMap.find(name) == spriteAnimationMap.end())
	{
		std::cout << "Can't find spriteAnimtion: " << name << "\n";
		std::cout << "Available animations: " << "\n";
		for (std::pair<std::string, const std::unique_ptr<SpriteAnimation>&> p : spriteAnimationMap)
		{
			std::cout << "animation: " << p.first << "\n";
		}
	}

	return spriteAnimationMap[name].get();
}

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
		std::cout << "Available textures: "<< "\n";
        for (std::pair<std::string, const std::unique_ptr<Texture>&> p : textureMap)
        {
            std::cout << "texture: " << p.first << "\n";
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

std::vector<Shader*> ResourceManager::GetAllShaders()
{
	std::vector<Shader*> result;
	result.reserve(shaderMap.size());
	for (auto& p : shaderMap)
	{
		result.emplace_back(p.second.get());
	}

	return result;
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

void ResourceManager::LoadAudioClip(const char* audioClipPath)
{
	std::string fullPath = std::string("../QkEngine/Resources/Audio/") + std::string(audioClipPath);
	std::cout << "Loading audioClip: " << fullPath.c_str() << "\n";
	irrklang::ISoundSource* audioClip = AudioManager::GetSoundEngine()->addSoundSourceFromFile(fullPath.c_str(), irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);

	audioClipMap[audioClipPath] = audioClip;
}

irrklang::ISoundSource* ResourceManager::GetAudioClip(const char* audioClipPath)
{
	if (audioClipMap.find(audioClipPath) == audioClipMap.end())
		std::cout << "Can't find audio clip: " << audioClipPath << "\n";

	return audioClipMap[audioClipPath];
}
