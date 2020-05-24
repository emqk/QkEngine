#pragma once
#include <glm\ext\vector_float3.hpp>
#include "Renderer.h"
#include "ResourceManager.h"

class Gizmos
{
public:
	Gizmos() = delete;
	~Gizmos() = delete;
	
	static void DrawCubeWireframe(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
};