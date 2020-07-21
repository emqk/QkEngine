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

unsigned int Renderer::framebuffer;
unsigned int Renderer::textureColorbuffer;
size_t Renderer::drawCallsLastFrame = 0;
size_t Renderer::drawVerticesLastFrame = 0;
std::vector<SpriteComponent*> Renderer::spriteComponents;

Texture* Renderer::defaultSpecularTexture = nullptr;

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


    //Defaults
    defaultSpecularTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    if (defaultSpecularTexture == nullptr)
        assert("defaultSpecularTexture is nullptr!");
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

void Renderer::DrawNew()
{
    drawCallsLastFrame = 0;
    drawVerticesLastFrame = 0;
    
    //Set global values to all shaders

    glm::vec3 fogColor = Lighting::GetFogColor();
    float fogDensity = Lighting::GetFogDensity();
    glm::vec2 fogStartEnd = Scene::GetCurrentScene().GetCamera().GetClipping();
    LightComponent* light = Lighting::GetFirstLight();
    glm::vec3 lightDirection = light == nullptr ? glm::vec3(-1, 0, 0) : light->GetParent()->transform.GetForward();
    glm::vec3 lightColor = light == nullptr ? glm::vec3(1, 1, 1) : light->GetColor();
    glm::vec3 ambientLightColor = Lighting::GetAmbientLightColor();
    glm::vec3 viewPos = Scene::GetCurrentScene().GetCamera().GetLocalPosition();

    std::vector<Shader*> allShaders = ResourceManager::GetAllShaders();
    for (Shader* shader : allShaders)
    {
        shader->Use();
        //Fog
        shader->SetVec3("_FogColor", fogColor.x, fogColor.y, fogColor.z);
        shader->SetFloat("near", fogStartEnd.x);
        shader->SetFloat("far", fogStartEnd.y);
        shader->SetFloat("_FogDensity", fogDensity);

        shader->SetVec3("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
        shader->SetVec3("light.color", lightColor.x, lightColor.y, lightColor.z);
        shader->SetVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);
        shader->SetVec3("material.ambient", ambientLightColor.x, ambientLightColor.y, ambientLightColor.z);
        shader->SetMat4("projection", Scene::GetCurrentScene().GetCamera().projection);
        shader->SetMat4("view", Scene::GetCurrentScene().GetCamera().view);
    }

    for (SpriteComponent* comp : spriteComponents)
    {
        if (!comp->IsActive())
            continue;

        Mesh* componentMeshNew = comp->GetMeshNew();
        const Texture* componentTexture = comp->GetTexture();
        const Texture* specularTexture = comp->GetSpecularTexture();
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

        //Default
        if (specularTexture == nullptr)
        {
            specularTexture = defaultSpecularTexture;
        }

        //Material
        componentShader->Use();

        glm::vec3 specular = comp->specular;

        componentShader->SetVec4("material.diffuse", comp->color.r, comp->color.g, comp->color.b, comp->color.a);
        componentShader->SetVec3("material.specularColor", specular.x, specular.y, specular.z);
        componentShader->SetFloat("material.shininess", comp->shininess);
        componentShader->SetInt("material.texture_diffuse1", 0);
        componentTexture->Use();

        //Specular texture
        componentShader->SetInt("material.texture_specular1", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture->GetID());

        //glm::mat4 model = Transform::CalculateModel(comp->GetParent());
        glm::mat4 model = comp->GetParent()->transform.GetModel();
        componentShader->SetMat4("model", model);


        // draw mesh
        glBindVertexArray(componentMeshNew->GetVAO());
        glDrawElements(GL_TRIANGLES, componentMeshNew->GetIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);


        ++drawCallsLastFrame;
        drawVerticesLastFrame += componentMeshNew->GetVertices().size();
    }
}

void Renderer::DrawMeshNewAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color)
{
    //Set wireframe mode only for this mesh
    int polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //float sinCalc = cos(timeValue);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, localScale);

    componentShader.SetMat4("model", model);
    componentShader.SetVec4("_FragColor", color.r, color.g, color.b, color.a);

    // draw mesh
    glBindVertexArray(componentMesh.GetVAO());
    glDrawElements(GL_TRIANGLES, componentMesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    //Go back to draw mode before this mesh
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void Renderer::AddSpriteToDraw(SpriteComponent* comp)
{
    spriteComponents.push_back(comp);
    //std::cout << "SpriteCompnent added to 'To Draw' container\n";
}

void Renderer::RemoveSpriteToDraw(SpriteComponent* comp)
{
    std::vector<SpriteComponent*>::iterator it = std::find(spriteComponents.begin(), spriteComponents.end(), comp);
    if (it != spriteComponents.end())
    {
        spriteComponents.erase(it);
        //std::cout << "SpriteComponent removed from renderer: " << spriteComponents.size() << "\n";
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