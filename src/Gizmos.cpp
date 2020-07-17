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

void Gizmos::DrawMeshNewWireframe(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& mesh)
{
    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("UnlitShader");

    Renderer::DrawMeshNewAtLocation(pos, rot, localScale, mesh, *componentTexture, *componentShader, currentColor);
}

void Gizmos::DrawCubeWireframe(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale)
{
    Mesh* componentMesh = ResourceManager::GetMeshNew("Cube.obj->Cube");
    const Texture* componentTexture = ResourceManager::GetTexture("gizmoSelectTexture.jpg");
    Shader* componentShader = ResourceManager::GetShader("UnlitShader");

    std::cout << "DrawCubeWireframe color: " << currentColor.r << "r " << currentColor.g << "g " << currentColor.b << "b\n";

    Renderer::DrawMeshNewAtLocation(pos, rot, localScale, *componentMesh, *componentTexture, *componentShader, currentColor);
}