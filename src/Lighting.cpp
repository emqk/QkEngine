#include "Lighting.h"
#include "imgui/imgui.h"
#include "Camera.h"
#include "Scene.h"

glm::vec3 Lighting::fogColor{ 1.0f, 1.0f, 1.0f };
float Lighting::fogDensity = 1.0f;

void Lighting::ShowWindow()
{
	ImGui::Begin("Lighting");

	ImGui::ColorEdit3("Fog color", &fogColor.r, ImGuiColorEditFlags_NoInputs);
	ImGui::InputFloat("Fog density", &fogDensity, 1, 10, 3);

	ImGui::End();
}

glm::vec3 Lighting::GetFogColor()
{
	return fogColor;
}

float Lighting::GetFogDensity()
{
	return fogDensity;
}
