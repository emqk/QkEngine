#include "ParticleSystem.h"
#include "../Random.h"

ParticleSystem::ParticleSystem(Texture* tex, Mesh* mesh, Shader* shader, const int& amount)
{
	particleTexture = tex;
	particleMesh = mesh;
	particleShader = shader;
	particles.reserve(amount);

	for (size_t i = 0; i < amount; i++)
	{
		glm::vec3 particleDirection = glm::normalize(glm::vec3(Random::RandomFloat(-100, 100), Random::RandomFloat(-100, 100), Random::RandomFloat(-100, 100)));
		float speed = Random::RandomFloat(0.1f, 0.1f);
		particles.emplace_back(std::make_unique<Particle>(particleDirection, speed, glm::vec4(1, 1, 1, 1)));
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(const float& deltaTime)
{
	for (const std::unique_ptr<Particle>& particle : particles)
	{
		particle->Update(deltaTime);
	}
}

const std::vector<std::unique_ptr<Particle>>& ParticleSystem::GetParticles() const
{
	return particles;
}

Texture* ParticleSystem::GetTexture() const
{
	return particleTexture;
}

Mesh* ParticleSystem::GetMesh() const
{
	return particleMesh;
}

Shader* ParticleSystem::GetShader() const
{
	return particleShader;
}
