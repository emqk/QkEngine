#pragma once
#include <random>

class Random
{
public:
	Random() = delete;
	~Random() = delete;

	static float RandomFloat(const float& min, const float& max);

private:

	static std::random_device random_device;
	static std::mt19937 random_engine;
};