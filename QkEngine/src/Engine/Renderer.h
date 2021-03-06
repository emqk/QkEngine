#pragma once
class GameObject;
class Widget;
class Shader;

#include "Components/StaticMeshComponent.h"
#include "UI/Widget.h"
#include "Particles/ParticleManager.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>

class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	static void Draw();
	static void DrawMeshAtLocation(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& localScale, const Mesh& componentMesh, const Texture& componentTexture, const Shader& componentShader, const glm::vec4& color, const bool& wirefame = true);

	static void DrawParticles();

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
	static const size_t maxPointLights = 16;

private:
	static void DrawParticleSystem(const ParticleSystem* particleSystem);

	static unsigned int framebuffer;
	static unsigned int textureColorbuffer;

	//Buffers for UI
	static unsigned int UI_VBO, UI_VAO, UI_EBO;
	//Buffers for Particles
	static unsigned int P_VBO, P_VAO, P_EBO;

	static size_t drawCallsLastFrame;
	static size_t drawVerticesLastFrame;
	static size_t enabledDirectionalLightsLastFrame;
	static size_t enabledPointLightsLastFrame;

	static std::vector<StaticMeshComponent*> staticMeshComponents;

	//Defaults
	static Texture* defaultSpecularTexture;
};