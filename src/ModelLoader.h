#pragma once
#include "Bounds.h"
#include <fstream>

struct MeshData
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<int> faces;

	Bounds bounds;
};

class ModelLoader
{
public:
	ModelLoader() = delete;
	~ModelLoader() = delete;

	static MeshData LoadObj(const char* path);
	
private:
	static void GetUntil(std::ifstream& fileStream, std::string& character);
	static void SplitToArray(std::string& source, std::string arr[]);
	static void OrganizeVertices(MeshData& meshData);
};