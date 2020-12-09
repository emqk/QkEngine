#include "Random.h"

std::random_device Random::random_device;
std::mt19937 Random::random_engine(random_device());

float Random::RandomFloat(const float& min, const float& max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random_device);
}

float Random::RandomFloat(const std::uniform_real_distribution<float>& distr)
{
	return distr(random_device);
}
