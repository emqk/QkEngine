#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <vector>

#include "Components/Lighting/DirectionalLightComponent.h"
#include "Components/Lighting/PointLightComponent.h"


class Lighting
{
public:
	Lighting() = delete;
	~Lighting() = delete;

	static void ShowWindow();
	
	static void RegisterPointLight(PointLightComponent* lightComp);
	static void UnRegisterPointLight(PointLightComponent* lightComp);
	static void RegisterLight(DirectionalLightComponent* lightComp);
	static void UnRegisterLight(DirectionalLightComponent* lightComp);

	static glm::vec3 GetAmbientLightColor();
	static DirectionalLightComponent* GetFirstLight();
	static const std::vector<PointLightComponent*>& GetPointLights();

	static glm::vec3 GetFogColor();
	static float GetFogDensity();

private:
	static glm::vec3 ambientLightColor;

	static glm::vec3 fogColor;
	static float fogDensity;

	static std::vector<DirectionalLightComponent*> lights;
	static std::vector<PointLightComponent*> pointLights;
};