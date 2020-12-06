#pragma once
#include <unordered_map>
#include <chrono>

struct ProfileData
{
	std::vector<float> histogramTimes;
};

class Profiler
{
public:
	Profiler() = delete;
	~Profiler() = delete;

	static void ShowData();

	static void BeginSample(const std::string& name);
	static void EndSample();

private:
	static std::unordered_map<std::string, ProfileData> datas;

	static std::string currentSampleName;
	static std::chrono::steady_clock::time_point currentSampleStartTime;

	static const size_t profilerHistogramSize = 100;
};