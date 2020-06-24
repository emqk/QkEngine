#include "ResourceManager.h"
#include "ModelLoader.h"
#include <iostream>

class MeshData;

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::textureMap;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourceManager::meshMap;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaderMap;

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

void ResourceManager::LoadMeshNew(const char* meshPath)
{
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
    std::string fullPath = std::string("../QkEngine/Resources/") + std::string(meshPath);
    std::cout << "Loading mesh NEW: " << fullPath.c_str() << "\n";

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(fullPath.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }    aiMesh* model = scene->mMeshes[0];
    std::cout << "Start loading meshes: " << scene->mNumMeshes << std::endl;
    MeshData meshData = ProcessMesh(model, scene);
    mesh->SetVertices(meshData.vertices);
    mesh->SetIndices(meshData.indices);
    mesh->SetBounds(meshData.bounds);

    std::cout << "MeshNEW loaded. Vert size: " << mesh->GetVertices().size() << "\n";

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

MeshData ResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
   // std::vector<Vertex> vertices;
    //std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> bitangents;
    std::vector<float> uvs;
    std::vector<int> faces;

    Bounds bounds;
    std::cout << "Start loading verts: " << mesh->mNumVertices << std::endl;
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        //Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        //vertex.Position = vector;
        vertices.push_back(vector.x);
        vertices.push_back(vector.y);
        vertices.push_back(vector.z);
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        normals.push_back(vector.x);
        normals.push_back(vector.y);
        normals.push_back(vector.z);
        //vertex.Normal = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            uvs.push_back(vector.x);
            uvs.push_back(vector.y);
            //vertex.TexCoords = vec;
        }
        else
        {
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
            //vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        // tangent
        //vector.x = mesh->mTangents[i].x;
        //vector.y = mesh->mTangents[i].y;
        //vector.z = mesh->mTangents[i].z;
        //tangents.push_back(vector.x);
        //tangents.push_back(vector.y);
        //tangents.push_back(vector.z);
       // vertex.Tangent = vector;
        // bitangent
        //vector.x = mesh->mBitangents[i].x;
        //vector.y = mesh->mBitangents[i].y;
        //vector.z = mesh->mBitangents[i].z;
        //bitangents.push_back(vector.x);
        //bitangents.push_back(vector.y);
        //bitangents.push_back(vector.z);
        //vertex.Bitangent = vector;
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    //aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    //// 1. diffuse maps
    //std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //// 2. specular maps
    //std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //// 3. normal maps
    //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //// 4. height maps
    //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    //return Mesh(vertices, indices, textures);
    MeshData data;
    data.vertices = vertices;
    data.indices = indices;
    data.normals = normals;
    data.uvs = uvs;
    data.faces = faces;
    return data;
}