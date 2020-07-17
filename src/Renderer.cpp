#include "Renderer.h"

#include <glad/glad.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\vector_float3.hpp>

#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Lighting.h"
#include "Scene.h"
#include "Window.h"

#include <vector>

unsigned int Renderer::VAO;
unsigned int Renderer::VBO;
unsigned int Renderer::EBO;
unsigned int Renderer::framebuffer;
unsigned int Renderer::textureColorbuffer;
size_t Renderer::drawCallsLastFrame = 0;
size_t Renderer::drawVerticesLastFrame = 0;
std::vector<SpriteComponent*> Renderer::spriteComponents;

void Renderer::Init()
{
    glm::vec2 winSize = Window::GetCurrentWindow()->GetWindowSize();
    // framebuffer configuration
// -------------------------
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winSize.x, winSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winSize.x, winSize.y); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Pre()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

}

void Renderer::Post()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Renderer::GetFrameBufferTextureID()
{
    return textureColorbuffer;
}


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
        //Fog
        glm::vec3 fogColor = Lighting::GetFogColor();
        float fogDensity = Lighting::GetFogDensity();
        glm::vec2 fogStartEnd = Scene::GetCurrentScene().GetCamera().GetClipping();
        componentShader->SetVec3("_FogColor", fogColor.x, fogColor.y, fogColor.z);
        componentShader->SetFloat("near", fogStartEnd.x);
        componentShader->SetFloat("far", fogStartEnd.y);
        componentShader->SetFloat("_FogDensity", fogDensity);
        //Lighting
        LightComponent* light = Lighting::GetFirstLight();
        glm::vec3 lightColor = light == nullptr ? glm::vec3(1, 1, 1) : light->GetColor();
        componentShader->SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
        //
        componentShader->SetInt("texture_diffuse1", 0);
        componentTexture->Use();

        BindMeshNew(*componentMeshNew);
        cameraShader.Use();

        glm::mat4 model = Transform::CalculateModel(comp->GetParent());
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
    int polygonMode;
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