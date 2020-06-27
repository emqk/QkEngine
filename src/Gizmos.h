#pragma once
#include <glm\ext\vector_float3.hpp>
#include "Renderer.h"
#include "ResourceManager.h"

class Gizmos
{
public:
	Gizmos() = delete;
	~Gizmos() = delete;
	
	static void SetCurrentColor(const glm::vec4& color);
	static void ResetDefaultColor();

	static void DrawMeshNewWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const MeshNew& mesh);
	static void DrawCubeWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

	static constexpr glm::vec4 meshWireframeColor = glm::vec4(1, 0, 0, 1);
	static constexpr glm::vec4 collisionColor = glm::vec4(0, 1, 0, 1);
	static constexpr glm::vec4 defaultColor = glm::vec4(1, 1, 1, 1);

private:
	static glm::vec4 currentColor;
};