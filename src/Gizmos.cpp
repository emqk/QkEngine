#include "Gizmos.h"
#include "Camera.h"
#include "Scene.h"


void Gizmos::DrawMeshWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, Mesh& mesh)
{
    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("StandardShader");

    Camera* cam = &Scene::GetCurrentScene().GetCamera();
    Renderer::DrawMeshAtLocation(pos, rot, scale, cam->GetShader(), mesh, *componentTexture, *componentShader, meshWireframeColor);
}

void Gizmos::DrawCubeWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
    Mesh* componentMesh = ResourceManager::GetMesh("Cube.obj");
    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("StandardShader");

    Camera* cam = &Scene::GetCurrentScene().GetCamera();
    Renderer::DrawMeshAtLocation(pos, rot, scale, cam->GetShader(), *componentMesh, *componentTexture, *componentShader, boundsColor);
}