#pragma once
#include <unordered_map>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Editor.h"

#include "NewModel.h"

class MeshData;

class ResourceManager
{
public:
	ResourceManager() = delete;
	~ResourceManager() = delete;

	static void LoadTexture(const char* texturePath);
	static Texture* GetTexture(const char* texturePath);
	static std::vector<std::string> GetTexturesName();

	static void LoadMesh(const char* meshPath);
	static void LoadMeshNew(const char* meshPath);
	static Mesh* GetMesh(const char* meshPath);
	static std::vector<std::string> GetMeshesName();

	static void LoadShader(const char* shaderPath);
	static Shader* GetShader(const char* shaderPath);
	static std::vector<std::string> GetShadersName();

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
	static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;

	static MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
};