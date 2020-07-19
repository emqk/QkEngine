#pragma once
class GameObject;
class Shader;

#include "Components/SpriteComponent.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>

class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	static void DrawNew();
	static void DrawMeshNewAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color);

	static void AddSpriteToDraw(SpriteComponent* comp);
	static void RemoveSpriteToDraw(SpriteComponent* comp);

	static size_t GetToDrawContainerSize();
	static size_t GetDrawCallsLastFrame();
	static size_t GetDrawnVerticesLastFrame();

	static void Init();
	static void Pre();
	static void Post();

	static unsigned int GetFrameBufferTextureID();

private:
	static unsigned int /*VBO, VAO, EBO, */framebuffer;
	static unsigned int textureColorbuffer;

	static size_t drawCallsLastFrame;
	static size_t drawVerticesLastFrame;


	static std::vector<SpriteComponent*> spriteComponents;
};