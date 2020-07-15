#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>

class Lighting
{
public:
	Lighting() = delete;
	~Lighting() = delete;

	static void ShowWindow();
	
	static glm::vec3 GetFogColor();
	static float GetFogDensity();

private:
	static glm::vec3 fogColor;
	static float fogDensity;

};