#pragma once
#include <random>

class Random
{
public:
	Random() = delete;
	~Random() = delete;

	static float RandomFloat(const float& min, const float& max);
	static float RandomFloat(const std::uniform_real_distribution<float>& distr);

private:

	static std::random_device random_device;
	static std::mt19937 random_engine;
};