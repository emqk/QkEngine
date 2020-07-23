#include "Profiler.h"
#include "Scene.h"

#include <string>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

std::unordered_map<std::string, ProfileData> Profiler::datas;

std::string Profiler::currentSampleName;
std::chrono::steady_clock::time_point Profiler::currentSampleStartTime;

void Profiler::ShowData()
{
	ImGui::Begin("Profiler");

	//Text stats
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Number of GameObjects: %u", Scene::GetCurrentScene().GetObjectsPtr()->size());
	ImGui::Text("Last frame draw calls: %u | To draw container size: %u", Renderer::GetDrawCallsLastFrame(), Renderer::GetToDrawContainerSize());
	ImGui::Text("Last frame vertices: %u", Renderer::GetDrawnVerticesLastFrame());
	ImGui::Text("Last frame active directional lights: %u / %u", Renderer::GetEnabledDirectionalLightsLastFrame(), Renderer::maxDirectionalLights);
	ImGui::Text("Last frame active point lights: %u / %u", Renderer::GetEnabledPointLightsLastFrame(), Renderer::maxPointLights);

	//Histograms
	int i = 0;
	for (const std::pair<std::string, ProfileData>& p : datas)
	{
		const std::vector<float>& currentTimes = p.second.histogramTimes;
		ImGui::PlotHistogram(p.first.c_str(), currentTimes.data(), currentTimes.size(), 0, (std::to_string(currentTimes.back()) + std::string("ms")).c_str(), 0.0f, 17.0f, ImVec2(375, 85));
		if (i % 2 == 0)
		{
			ImGui::SameLine();
		}
		i++;
	}
	
	ImGui::End();
}

void Profiler::BeginSample(const std::string& name)
{
	currentSampleStartTime = std::chrono::steady_clock::now();
	currentSampleName = name;

	if (datas.empty())
	{
		auto& newMapElement = datas[name] = ProfileData{ std::vector<float>(profilerHistogramSize) };
	}
	else
	{
		auto it = datas.find(name);
		if (it == datas.end())
		{
			datas[name] = ProfileData{ std::vector<float>(profilerHistogramSize) };
		}
	}
}

void Profiler::EndSample()
{
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> dur = endTime - currentSampleStartTime;
	float sampleDuration = dur.count();

	std::vector<float>& histogramData = datas[currentSampleName].histogramTimes;
	std::rotate(histogramData.begin(), histogramData.begin() + 1, histogramData.end());
	histogramData.back() = sampleDuration;
}