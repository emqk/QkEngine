#pragma once
class GameObject;
class Widget;
class Shader;

#include "Components/StaticMeshComponent.h"
#include "UI/Widget.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>

class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	static void DrawNew();
	static void DrawMeshNewAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color);

	static void PrepareDrawUI();
	static void DrawUI(const Widget* widget);
	static void EndDrawUI();

	static void AddSpriteToDraw(StaticMeshComponent* comp);
	static void RemoveSpriteToDraw(StaticMeshComponent* comp);

	static size_t GetToDrawContainerSize();
	static size_t GetDrawCallsLastFrame();
	static size_t GetDrawnVerticesLastFrame();
	static size_t GetEnabledDirectionalLightsLastFrame();
	static size_t GetEnabledPointLightsLastFrame();

	static void Init();
	static void Pre();
	static void Post();

	static unsigned int GetFrameBufferTextureID();

	static const size_t maxDirectionalLights = 1;
	static const size_t maxPointLights = 4;

private:
	static unsigned int framebuffer;
	static unsigned int textureColorbuffer;

	//Buffers for UI
	static unsigned int UI_VBO, UI_VAO, UI_EBO;

	static size_t drawCallsLastFrame;
	static size_t drawVerticesLastFrame;
	static size_t enabledDirectionalLightsLastFrame;
	static size_t enabledPointLightsLastFrame;

	static std::vector<StaticMeshComponent*> StaticMeshComponents;

	//Defaults
	static Texture* defaultSpecularTexture;
};