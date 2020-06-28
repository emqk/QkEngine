#pragma once
//#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshNew.h"

#include <string>
#include <iostream>
#include <map>
#include "stb_image.h"

//unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // model data 
    //vector<Texture*> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    //std::vector<std::string> textures_loaded;	// Own string version
    std::vector<Mesh>    meshes;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(const std::string & path, const std::string& partPath);
    const std::string& GetDirectory() const;
    const std::string& GetShortDirectory() const;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    //vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    //std::vector<std::string> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); // Own string version

    std::string directory;
    std::string shortDirectory;
    Bounds bounds;
};