#pragma once
class GameObject;
class Shader;

#include "Components/SpriteComponent.h"
#include <vector>

class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	static void DrawNew(Shader& cameraShader);
	static void DrawMeshNewAtLocation(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, Shader& cameraShader, const MeshNew& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color);

	static void AddSpriteToDraw(SpriteComponent* comp);
	static void RemoveSpriteToDraw(SpriteComponent* comp);

	static size_t GetToDrawContainerSize();
	static size_t GetDrawCallsLastFrame();
	static size_t GetDrawnVerticesLastFrame();

private:
	static void BindMeshNew(const MeshNew& mesh);
	static unsigned int VBO, VAO, EBO;
	
	static size_t drawCallsLastFrame;
	static size_t drawVerticesLastFrame;

	static std::vector<SpriteComponent*> spriteComponents;
};