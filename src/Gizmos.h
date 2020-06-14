#pragma once
#include <glm\ext\vector_float3.hpp>
#include "Renderer.h"
#include "ResourceManager.h"

class Gizmos
{
public:
	Gizmos() = delete;
	~Gizmos() = delete;
	
	static void DrawMeshWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, Mesh& mesh);
	static void DrawCubeWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

private:
	static constexpr glm::vec4 meshWireframeColor = glm::vec4(1, 0, 0, 1);
	static constexpr glm::vec4 boundsColor = glm::vec4(0, 1, 0, 1);
};