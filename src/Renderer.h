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

	static void Draw(Shader& cameraShader);

	static void AddSpriteToDraw(SpriteComponent* comp);
	static void RemoveSpriteToDraw(SpriteComponent* comp);

	static size_t GetToDrawContainerSize();
	static size_t GetDrawCallsLastFrame();

private:
	static void BindMesh(float vert[], const unsigned int& vertSize);
	static unsigned int VBO, VAO;
	
	static size_t drawCallsLastFrame;

	static std::vector<SpriteComponent*> spriteComponents;
};