#include "Renderer.h"

#include <glad/glad.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\vector_float3.hpp>

#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"

#include <vector>

unsigned int Renderer::VAO;
unsigned int Renderer::VBO;
unsigned int Renderer::EBO;
size_t Renderer::drawCallsLastFrame = 0;
size_t Renderer::drawVerticesLastFrame = 0;
std::vector<SpriteComponent*> Renderer::spriteComponents;


void Renderer::BindMeshNew(const Mesh& mesh)
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
    glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(Vertex), &mesh.GetVertices()[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int), &mesh.GetIndices()[0], GL_STATIC_DRAW);

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

glm::mat4 Renderer::CalculateModel(const GameObject const* obj)
{
    glm::mat4 model = glm::mat4(1.0f);
    CalculateModel(obj, model);
    return model;
}

void Renderer::CalculateModel(const GameObject const* obj, glm::mat4& model)
{
    if (obj->GetParent() != nullptr)
    {
        CalculateModel(obj->GetParent(), model);
        model *= obj->GetTransform().GetLocalMatrix();
    }
    else
    {
        model = obj->GetTransform().GetLocalMatrix();
    }
}

void Renderer::DrawNew(Shader& cameraShader)
{
    drawCallsLastFrame = 0;
    drawVerticesLastFrame = 0;

    for (SpriteComponent* comp : spriteComponents)
    {
        if (!comp->IsActive())
            continue;

        Mesh* componentMeshNew = comp->GetMeshNew();
        const Texture* componentTexture = comp->GetTexture();
        Shader* componentShader = comp->GetShader();

        if (componentMeshNew == nullptr)
        {
            //std::cout << "Can't render SpriteComponent: Mesh is null!\n";
            continue;
        }
        if (componentTexture == nullptr)
        {
            //std::cout << "Can't render SpriteComponent: Texture is null!\n";
            continue;
        }
        if (componentShader == nullptr)
        {
            //std::cout << "Can't render SpriteComponent: Shader is null!\n";
            continue;
        }
        if (comp->GetParent() == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Parent is null!\n";
            continue;
        }

        componentShader->SetVec4("_FragColor", comp->color.r, comp->color.g, comp->color.b, comp->color.a);
        componentShader->SetInt("texture_diffuse1", 0);
        componentTexture->Use();

        BindMeshNew(*componentMeshNew);
        cameraShader.Use();

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //float sinCalc = cos(timeValue);
        CalculateModel(comp->GetParent(), model);
        componentShader->SetMat4("model", model);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, componentMeshNew->GetIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        ++drawCallsLastFrame;
        drawVerticesLastFrame += componentMeshNew->GetVertices().size();
    }
}

void Renderer::DrawMeshNewAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, Shader& cameraShader, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color)
{
    //Set wireframe mode only for this mesh
    GLint polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    componentShader.SetVec4("_FragColor", color.r, color.g, color.b, color.a);
    componentShader.SetInt("texture_diffuse1", 0);
    componentTexture.Use();

    BindMeshNew(componentMesh);
    cameraShader.Use();

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //float sinCalc = cos(timeValue);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, localScale);
    componentShader.SetMat4("model", model);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, componentMesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //Go back to draw mode before this mesh
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void Renderer::AddSpriteToDraw(SpriteComponent* comp)
{
    spriteComponents.push_back(comp);
    std::cout << "SpriteCompnent added to 'To Draw' container\n";
}

void Renderer::RemoveSpriteToDraw(SpriteComponent* comp)
{
    std::vector<SpriteComponent*>::iterator it = std::find(spriteComponents.begin(), spriteComponents.end(), comp);
    if (it != spriteComponents.end())
    {
        spriteComponents.erase(it);
        std::cout << "SpriteComponent removed from renderer: " << spriteComponents.size() << "\n";
    }
}

size_t Renderer::GetToDrawContainerSize()
{
    return spriteComponents.size();
}

size_t Renderer::GetDrawCallsLastFrame()
{
    return drawCallsLastFrame;
}

size_t Renderer::GetDrawnVerticesLastFrame()
{
    return drawVerticesLastFrame;
}