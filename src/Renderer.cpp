#include "Renderer.h"

#include <glad/glad.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\vector_float3.hpp>

#include "GameObject.h"
#include "Shader.h"

#include <vector>

unsigned int Renderer::VAO;
unsigned int Renderer::VBO;
unsigned int Renderer::EBO;
size_t Renderer::drawCallsLastFrame = 0;
size_t Renderer::drawTrianglesLastFrame = 0;
std::vector<SpriteComponent*> Renderer::spriteComponents;

void Renderer::BindMesh(float vert[], const unsigned int& vertSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vert, GL_STATIC_DRAW);

    //Vert coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);
}

void Renderer::BindMeshNew(const MeshNew& mesh)
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
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

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

void Renderer::Draw(Shader& cameraShader)
{
    drawCallsLastFrame = 0;
    drawTrianglesLastFrame = 0;

    for (SpriteComponent* comp : spriteComponents)
    {
        if (!comp->IsActive())
            continue;

        Mesh* componentMesh = comp->GetMesh();
        const Texture* componentTexture = comp->GetTexture();
        Shader* componentShader = comp->GetShader();

        if (componentMesh == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Mesh is null!\n";
            continue;
        }
        if (componentTexture == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Texture is null!\n";
            continue;
        }
        if (componentShader == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Shader is null!\n";
            continue;
        }
        if (comp->GetParent() == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Parent is null!\n";
            continue;
        }

        componentShader->SetVec4("_FragColor", comp->color.r, comp->color.g, comp->color.b, comp->color.a);
        componentShader->SetInt("texture1", 0);
        componentTexture->Use();

        std::vector<float> verts = componentMesh->GetVertices();

        BindMesh(verts.data(), verts.size() * sizeof(float));
        cameraShader.Use();

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //float sinCalc = cos(timeValue);
        model = glm::translate(model, comp->GetParent()->GetPosition());
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().x * 360), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().y * 360), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().z * 360), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, comp->GetParent()->GetScale());
        componentShader->SetMat4("model", model);


        glDrawArrays(GL_TRIANGLES, 0, verts.size());
        // optional: de-allocate all resources once they've outlived their purpose:
        // ----------------------------------------------------------------------ss--
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        ++drawCallsLastFrame;
        drawTrianglesLastFrame += verts.size();
    }
}

void Renderer::DrawNew(Shader& cameraShader)
{
    for (SpriteComponent* comp : spriteComponents)
    {
        if (!comp->IsActive())
            continue;

        MeshNew* componentMeshNew = comp->GetMeshNew();
        const Texture* componentTexture = comp->GetTexture();
        Shader* componentShader = comp->GetShader();

        if (componentMeshNew == nullptr)
        {
            std::cout << "Can't render SpriteComponent: MeshNew is null!\n";
            continue;
        }
        if (componentTexture == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Texture is null!\n";
            continue;
        }
        if (componentShader == nullptr)
        {
            std::cout << "Can't render SpriteComponent: Shader is null!\n";
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
        model = glm::translate(model, comp->GetParent()->GetPosition());
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().x * 360), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().y * 360), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(comp->GetParent()->GetRotation().z * 360), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, comp->GetParent()->GetScale());
        componentShader->SetMat4("model", model);


        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, componentMeshNew->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void Renderer::DrawMeshAtLocation(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, Shader& cameraShader, Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color)
{
    GLint polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    componentShader.SetVec4("_FragColor", color.r, color.g, color.b, color.a);
    componentShader.SetInt("texture_diffuse1", 0);
    componentTexture.Use();

    std::vector<float> verts = componentMesh.GetVertices();

    BindMesh(verts.data(), verts.size() * sizeof(float));
    cameraShader.Use();

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //float sinCalc = cos(timeValue);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x * 360), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.y * 360), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.z * 360), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    componentShader.SetMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, verts.size());
    // optional: de-allocate all resources once they've outlived their purpose:
    // ----------------------------------------------------------------------ss--
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

   // std::cout << "Draw gizmo\n";
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

size_t Renderer::GetDrawnTrianglesLastFrame()
{
    return drawTrianglesLastFrame;
}
