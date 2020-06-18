#include "Gizmos.h"
#include "Camera.h"
#include "Scene.h"

glm::vec4 Gizmos::currentColor = Gizmos::defaultColor;

void Gizmos::SetCurrentColor(const glm::vec4& color)
{
    currentColor = color;
}

void Gizmos::ResetDefaultColor()
{
    currentColor = defaultColor;
}

void Gizmos::DrawMeshWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, Mesh& mesh)
{
    if (!Editor::CanDrawGizmos())
        return;

    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("StandardShader");

    Camera* cam = &Scene::GetCurrentScene().GetCamera();
    Renderer::DrawMeshAtLocation(pos, rot, scale, cam->GetShader(), mesh, *componentTexture, *componentShader, currentColor);
}

void Gizmos::DrawCubeWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
    if (!Editor::CanDrawGizmos())
        return;
    
    Mesh* componentMesh = ResourceManager::GetMesh("Cube.obj");
    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("StandardShader");

    Camera* cam = &Scene::GetCurrentScene().GetCamera();
    Renderer::DrawMeshAtLocation(pos, rot, scale, cam->GetShader(), *componentMesh, *componentTexture, *componentShader, currentColor);
}