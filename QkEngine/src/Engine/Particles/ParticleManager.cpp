#include "ParticleManager.h"

std::vector<std::unique_ptr<ParticleSystem>> ParticleManager::particleSystems;

void ParticleManager::SpawnEmitter(std::unique_ptr<ParticleSystem> particleSystem)
{
	particleSystems.emplace_back(std::move(particleSystem));
}

void ParticleManager::Update(const float& deltaTime)
{
	for (const std::unique_ptr<ParticleSystem>& pSystem : particleSystems)
	{
		pSystem->Update(deltaTime);
	}
}
