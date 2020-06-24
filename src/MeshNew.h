#pragma once

#include "Shader.h"
#include "Texture.h"

#include <string>
#include <vector>
using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

//struct TextureNew {
//    unsigned int id;
//    string type;
//    string path;
//};

class MeshNew {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture*>      textures;
    unsigned int VAO;

    // constructor
    MeshNew(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};