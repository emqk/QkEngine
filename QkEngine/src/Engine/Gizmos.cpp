#include "Gizmos.h"
#include "Camera.h"
#include "Scene.h"

glm::vec4 Gizmos::currentColor = Gizmos::defaultColor;
Shader* Gizmos::defaultShader = nullptr;
Texture* Gizmos::defaultTexture = nullptr;

void Gizmos::Init()
{
    defaultShader = ResourceManager::GetShader("UnlitShader");
    defaultTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
}

void Gizmos::SetCurrentColor(const glm::vec4& color)
{
    currentColor = color;
}

void Gizmos::ResetDefaultColor()
{
    currentColor = defaultColor;
}

void Gizmos::PrepareToRender()
{
    defaultShader->Use();
    defaultShader->SetInt("texture_diffuse1", 0);
    defaultTexture->Use();

    defaultShader->SetMat4("projection", Scene::GetCurrentScene().GetCamera().projection);
    defaultShader->SetMat4("view", Scene::GetCurrentScene().GetCamera().view);
}

void Gizmos::DrawMeshNewWireframe(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& mesh)
{
    Renderer::DrawMeshNewAtLocation(pos, rot, localScale, mesh, *defaultTexture, *defaultShader, currentColor);
}

void Gizmos::DrawCubeWireframe(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale)
{
    Mesh* componentMesh = ResourceManager::GetMeshNew("Cube.obj->Cube");
    Renderer::DrawMeshNewAtLocation(pos, rot, localScale, *componentMesh, *defaultTexture, *defaultShader, currentColor);
}