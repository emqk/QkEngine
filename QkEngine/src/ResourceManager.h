#pragma once
#include <unordered_map>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"
#include "Animation/SpriteAnimation.h"
#include "Shader.h"
#include "Editor.h"

#include "Model.h"

class ResourceManager
{
public:
	ResourceManager() = delete;
	~ResourceManager() = delete;

	static void LoadSpriteAnimation(std::vector<Texture*> _textures, const char* name);
	static SpriteAnimation* GetSpriteAnimation(const char* name);

	static void LoadTexture(const char* texturePath);
	static Texture* GetTexture(const char* texturePath);
	static std::vector<std::string> GetTexturesName();

	static void LoadModel(const char* meshPath);
	static Model* GetModel(const char* meshPath);
	static void LoadMesh(Model* model);
	static Mesh* GetMeshNew(const char* meshPath);
	static std::vector<std::string> GetMeshesNewName();

	static void LoadShader(const char* shaderPath);
	static std::vector<Shader*> GetAllShaders();
	static Shader* GetShader(const char* shaderPath);
	static std::vector<std::string> GetShadersName();

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;

	static std::unordered_map<std::string, std::unique_ptr<SpriteAnimation>> spriteAnimationMap;

	static std::unordered_map<std::string, std::unique_ptr<Model>> modelMap;
	static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshNewMap;
};