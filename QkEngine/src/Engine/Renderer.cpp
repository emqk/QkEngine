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
unsigned int Renderer::UI_VBO, Renderer::UI_VAO, Renderer::UI_EBO;
unsigned int Renderer::P_VBO, Renderer::P_VAO, Renderer::P_EBO;
size_t Renderer::drawCallsLastFrame = 0;
size_t Renderer::drawVerticesLastFrame = 0;
size_t Renderer::enabledDirectionalLightsLastFrame = 0;
size_t Renderer::enabledPointLightsLastFrame = 0;
std::vector<StaticMeshComponent*> Renderer::StaticMeshComponents;

Texture* Renderer::defaultSpecularTexture = nullptr;

void Renderer::Init()
{
    glm::vec2 winSize = Window::GetCurrentWindow()->GetWindowSize();

    //Defaults
    defaultSpecularTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    if (defaultSpecularTexture == nullptr)
        assert("defaultSpecularTexture is nullptr!");

    if (Window::IsItBuild())
        return;

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

void Renderer::DrawNew()
{
    drawCallsLastFrame = 0;
    drawVerticesLastFrame = 0;
    enabledDirectionalLightsLastFrame = 0;
    enabledPointLightsLastFrame = 0;
    
    //Set global values to all shaders
    glm::vec3 fogColor = Lighting::GetFogColor();
    float fogDensity = Lighting::GetFogDensity();
    glm::vec2 fogStartEnd = Scene::GetCurrentScene().GetCamera().GetClipping();
    //Directional light
    DirectionalLightComponent* directionalLight = Lighting::GetFirstLight();
    glm::vec3 dirLightDirection = glm::vec3(0, 0, 0);
    glm::vec3 dirLightColor = glm::vec3(0, 0, 0);
    if (directionalLight != nullptr)
    {
        dirLightDirection = directionalLight->GetParent()->transform.GetForward();
        dirLightColor = directionalLight->GetColor();
        ++enabledDirectionalLightsLastFrame;
    }
    //Point lights
    std::vector<PointLightComponent*> activePointLights;
    const std::vector<PointLightComponent*>& orgPointLights = Lighting::GetPointLights();
    for (size_t i = 0; i < Lighting::GetPointLights().size(); i++)
    {
        if (i < orgPointLights.size() && orgPointLights[i]->IsActive())
        {
            activePointLights.push_back(orgPointLights[i]);
            ++enabledPointLightsLastFrame;
        }
    }

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

        //Directional light
        if (directionalLight != nullptr)
        {
            shader->SetVec3("dirLight.direction", dirLightDirection.x, dirLightDirection.y, dirLightDirection.z);
            shader->SetVec3("dirLight.color", dirLightColor.x, dirLightColor.y, dirLightColor.z);
        }
        else
        {
            shader->SetVec3("dirLight.direction", 1,0,0);
            shader->SetVec3("dirLight.color", 0, 0, 0);
        }

        //Point lights
        //Clear
        for (size_t i = 0; i < maxPointLights; i++)
        {
            shader->SetVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), 0,0,0);
            shader->SetVec3(("pointLights[" + std::to_string(i) + "].color").c_str(), 0,0,0);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), 1.0f);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), 0.09f);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);
        }
        for (size_t i = 0; i < activePointLights.size(); i++)
        {
            PointLightComponent* pLight = activePointLights[i];
            glm::vec3 pLightPos = pLight->GetPosition();
            glm::vec3 pLightColor = pLight->GetColor();

            shader->SetVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), pLightPos.x, pLightPos.y, pLightPos.z);
            shader->SetVec3(("pointLights[" + std::to_string(i) + "].color").c_str(), pLightColor.x, pLightColor.y, pLightColor.z);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), 1.0f);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), 0.09f);
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);       
            shader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pLight->GetIntensity());       
        }

        shader->SetVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);
        shader->SetVec3("ambientColor", ambientLightColor.x, ambientLightColor.y, ambientLightColor.z);
        shader->SetMat4("projection", Scene::GetCurrentScene().GetCamera().projection);
        shader->SetMat4("view", Scene::GetCurrentScene().GetCamera().view);
    }

    for (StaticMeshComponent* comp : StaticMeshComponents)
    {
        if (!comp->IsActive())
            continue;

        Mesh* componentMeshNew = comp->GetMeshNew();
        const Texture* componentTexture = comp->GetTexture();
        const Texture* specularTexture = comp->GetSpecularTexture();
        Shader* componentShader = comp->GetShader();

        if (componentMeshNew == nullptr)
        {
            //std::cout << "Can't render StaticMeshComponent: Mesh is null!\n";
            continue;
        }
        if (componentTexture == nullptr)
        {
            //std::cout << "Can't render StaticMeshComponent: Texture is null!\n";
            continue;
        }
        if (componentShader == nullptr)
        {
            //std::cout << "Can't render StaticMeshComponent: Shader is null!\n";
            continue;
        }
        if (comp->GetParent() == nullptr)
        {
            std::cout << "Can't render StaticMeshComponent: Parent is null!\n";
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

void Renderer::DrawMeshNewAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color, const bool& wirefame)
{
    //Set wireframe mode only for this mesh
    int polygonMode = 0;
    if (wirefame)
    {
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

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
    if(wirefame)
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void Renderer::DrawParticles()
{
    for (const std::unique_ptr<ParticleSystem>& particleSystem : ParticleManager::particleSystems)
    {
        DrawParticleSystem(particleSystem.get());
    }
}

void Renderer::DrawParticleSystem(const ParticleSystem* particleSystem)
{
    //Draw
    glm::vec3 particleSystemPosition = particleSystem->GetPosition();
    glm::vec3 particleSystemScale = particleSystem->GetScale();

    Shader* shader = particleSystem->GetShader();
    glm::vec4 color = particleSystem->GetColor();
    shader->SetVec4("material.diffuse", color.r, color.g, color.b, color.a);
    shader->SetVec3("material.specularColor", 0.0f, 0.0f, 0.0f);
    shader->SetFloat("material.shininess", 32);
    shader->SetInt("material.texture_diffuse1", 0);
    particleSystem->GetTexture()->Use();

    //Specular texture
    shader->SetInt("material.texture_specular1", 1);
    glActiveTexture(GL_TEXTURE1);
    Texture* specTex = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    glBindTexture(GL_TEXTURE_2D, specTex->GetID());

    const std::vector<std::unique_ptr<Particle>>& particles = particleSystem->GetParticles();

    for (const std::unique_ptr<Particle>& particle : particles)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, particle->GetPosition() + particleSystemPosition);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, particleSystemScale);

        shader->SetMat4("model", model);

        // draw mesh
        glBindVertexArray(particleSystem->GetMesh()->GetVAO());
        glDrawElements(GL_TRIANGLES, particleSystem->GetMesh()->GetIndices().size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::PrepareDrawUI()
{
    float vertices[] = {
     1.0f, 1.0f, 0.0f,    1.0f, 1.0f, // top right
     1.0f,-1.0f, 0.0f,    1.0f, 0.0f, // bottom right
    -1.0f,-1.0f, 0.0f,    0.0f, 0.0f, // bottom left
    -1.0f, 1.0f, 0.0f,    0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &UI_VAO);
    glGenBuffers(1, &UI_VBO);
    glGenBuffers(1, &UI_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(UI_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, UI_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UI_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void Renderer::DrawUI(const Widget* widget)
{
    Shader* uiShader = ResourceManager::GetShader("UIShader");
    uiShader->Use();

    uiShader->SetInt("texture_diffuse1", 0);
    widget->GetTexture()->Use();

    glm::vec2 widgetPosition = widget->GetPosition();
    glm::vec2 widgetSize = widget->GetSize();
    uiShader->SetVec2("topLeft", widgetPosition.x, widgetPosition.y);
    uiShader->SetVec2("size", widgetSize.x, widgetSize.y);

    glUseProgram(uiShader->ID);
    glBindVertexArray(UI_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::EndDrawUI()
{
    glDeleteVertexArrays(1, &UI_VAO);
    glDeleteBuffers(1, &UI_VBO);
    glDeleteBuffers(1, &UI_EBO);
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::AddSpriteToDraw(StaticMeshComponent* comp)
{
    StaticMeshComponents.push_back(comp);
    //std::cout << "SpriteCompnent added to 'To Draw' container\n";
}

void Renderer::RemoveSpriteToDraw(StaticMeshComponent* comp)
{
    std::vector<StaticMeshComponent*>::iterator it = std::find(StaticMeshComponents.begin(), StaticMeshComponents.end(), comp);
    if (it != StaticMeshComponents.end())
    {
        StaticMeshComponents.erase(it);
        //std::cout << "StaticMeshComponent removed from renderer: " << StaticMeshComponents.size() << "\n";
    }
}

size_t Renderer::GetToDrawContainerSize()
{
    return StaticMeshComponents.size();
}

size_t Renderer::GetDrawCallsLastFrame()
{
    return drawCallsLastFrame;
}

size_t Renderer::GetDrawnVerticesLastFrame()
{
    return drawVerticesLastFrame;
}

size_t Renderer::GetEnabledDirectionalLightsLastFrame()
{
    return enabledDirectionalLightsLastFrame;
}

size_t Renderer::GetEnabledPointLightsLastFrame()
{
    return enabledPointLightsLastFrame;
}
