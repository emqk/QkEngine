#include "imgui/imgui.h"

#include "Lighting.h"
#include "Camera.h"
#include "Scene.h"

glm::vec3 Lighting::ambientLightColor{ 0.0f, 0.0f, 0.0f };

glm::vec3 Lighting::fogColor{ 1.0f, 1.0f, 1.0f };
float Lighting::fogDensity = 1.0f;

std::vector<PointLightComponent*> Lighting::pointLights;
std::vector<DirectionalLightComponent*> Lighting::lights;


void Lighting::ShowWindow()
{
	ImGui::Begin("Lighting");

	ImGui::Text("Light");
	ImGui::ColorEdit3("Ambient light color", &ambientLightColor.r, ImGuiColorEditFlags_NoInputs);
	ImGui::Separator();
	ImGui::Text("Fog");
	ImGui::ColorEdit3("Fog color", &fogColor.r, ImGuiColorEditFlags_NoInputs);
	ImGui::InputFloat("Fog density", &fogDensity, 1, 10, 3);

	ImGui::End();
}

void Lighting::RegisterPointLight(PointLightComponent* lightComp)
{
	pointLights.push_back(lightComp);
}

void Lighting::UnRegisterPointLight(PointLightComponent* lightComp)
{
	std::vector<PointLightComponent*>::iterator it = std::find(pointLights.begin(), pointLights.end(), lightComp);
	if (it != pointLights.end())
	{
		pointLights.erase(it);
		//std::cout << "DirectionalLightComponent removed from lights: " << lights.size() << "\n";
	}
}

void Lighting::RegisterLight(DirectionalLightComponent* lightComp)
{
	lights.push_back(lightComp);
	//std::cout << "DirectionalLightComponent registered\n";
}

void Lighting::UnRegisterLight(DirectionalLightComponent* lightComp)
{
	std::vector<DirectionalLightComponent*>::iterator it = std::find(lights.begin(), lights.end(), lightComp);
	if (it != lights.end())
	{
		lights.erase(it);
		//std::cout << "DirectionalLightComponent removed from lights: " << lights.size() << "\n";
	}
}

glm::vec3 Lighting::GetAmbientLightColor()
{
	return ambientLightColor;
}

//Returns first active light
DirectionalLightComponent* Lighting::GetFirstLight()
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->IsActive())
		{
			return lights[i];
		}
	}

	return nullptr;
}

const std::vector<PointLightComponent*>& Lighting::GetPointLights()
{
	return pointLights;
}

glm::vec3 Lighting::GetFogColor()
{
	return fogColor;
}

float Lighting::GetFogDensity()
{
	return fogDensity;
}
