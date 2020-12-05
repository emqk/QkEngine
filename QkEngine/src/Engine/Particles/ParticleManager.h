#pragma once
#include "ParticleSystem.h"

class ParticleManager
{
	friend class Renderer;

public:
	ParticleManager() = delete;
	~ParticleManager() = delete;

	static void SpawnEmitter(std::unique_ptr<ParticleSystem> particleSystem);
	static void Update(const float& deltaTime);

private:
	static std::vector<std::unique_ptr<ParticleSystem>> particleSystems;
};