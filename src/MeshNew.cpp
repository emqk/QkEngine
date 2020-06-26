#include "MeshNew.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Gizmos.h"
#include "ResourceManager.h"

MeshNew::MeshNew(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures, const Bounds& _bounds, const glm::vec3& _offset, const std::string& _name)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    bounds = _bounds;
    offset = _offset;
    name = _name;

    std::cout << "Mesh loaded!\n";
    std::cout << "vertices: " << this->vertices.size() << std::endl;
    std::cout << "indices: " << this->indices.size() << std::endl;
    std::cout << "textures: " << this->textures.size() << std::endl;
}

void MeshNew::Draw(Shader& shader)
{
    glm::vec3 extents = bounds.Extents();
    Gizmos::DrawCubeWireframe(offset, glm::vec3(0, 0, 0), extents);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    shader.SetMat4("model", model);

    //Auto 
    //std::cout << "draw textures: " << textures.size() << std::endl;
    // bind appropriate textures
    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;
    //unsigned int normalNr = 1;
    //unsigned int heightNr = 1;
    //for (unsigned int i = 0; i < textures.size(); i++)
    //{
    //    //glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //    // retrieve texture number (the N in diffuse_textureN)
    //    string number;
    //    string name = textures[i]->type;
    //    if (name == "texture_diffuse")
    //        number = std::to_string(diffuseNr++);
    //    else if (name == "texture_specular")
    //        number = std::to_string(specularNr++); // transfer unsigned int to stream
    //    else if (name == "texture_normal")
    //        number = std::to_string(normalNr++); // transfer unsigned int to stream
    //    else if (name == "texture_height")
    //        number = std::to_string(heightNr++); // transfer unsigned int to stream
    //
    //    // now set the sampler to the correct texture unit
    //    shader.SetInt((name + number).c_str(), 0);     // <---------------------- To display textures different than diffuse change 0 to other number
    //    textures[0]->Use();                            // <---------------------- To display textures different than diffuse change 0 to other number
    //}

    //Manual texture set
    shader.SetInt("texture_diffuse1", 0);     // <---------------------- To display textures different than diffuse change 0 to other number
    //Texture* diffTexture = ResourceManager::GetTexture("backpack/diffuse.jpg");
    Texture* diffTexture = ResourceManager::GetTexture("Chair/diffuse.png");
    diffTexture->Use();

    shader.SetVec4("_FragColor", 1.0f, 1.0f, 1.0f, 1.0f);
    //shader.SetInt("texture1", 0);
    shader.Use();

    setupMesh();

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void MeshNew::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
     //set the vertex attribute pointers
     //vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    //// vertex bitangent
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    
    glBindVertexArray(0);
}
