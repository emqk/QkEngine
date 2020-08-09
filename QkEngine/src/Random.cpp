#include "Random.h"

std::random_device Random::random_device;
std::mt19937 Random::random_engine(random_device());

float Random::RandomFloat(const float& min, const float& max)
{
	std::uniform_real_distribution<float> dist(min, max);
	float result = dist(random_device);
	return result;
}