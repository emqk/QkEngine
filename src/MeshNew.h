#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Bounds.h"

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

class MeshNew {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture*>      textures;

    std::string name;
    Bounds bounds;
    glm::vec3 offset;

    // constructor
    MeshNew(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures, const Bounds& _bounds, const glm::vec3& _offset, const std::string& _name);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VAO;
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};