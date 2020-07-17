#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <vector>

#include "Components/LightComponent.h"


class Lighting
{
public:
	Lighting() = delete;
	~Lighting() = delete;

	static void ShowWindow();
	
	static void RegisterLight(LightComponent* lightComp);
	static void UnRegisterLight(LightComponent* lightComp);

	static LightComponent* GetFirstLight();

	static glm::vec3 GetFogColor();
	static float GetFogDensity();

private:
	static glm::vec3 fogColor;
	static float fogDensity;

	static std::vector<LightComponent*> lights;

};