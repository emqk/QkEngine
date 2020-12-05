#include "ParticleSystem.h"
#include "../Random.h"

ParticleSystem::ParticleSystem(const glm::vec3& pos, Texture* tex, Mesh* mesh, Shader* shader, const glm::vec4& _color, const int& amount) 
	: position(pos), particleTexture(tex), particleMesh(mesh), particleShader(shader), color(_color)
{
	particles.reserve(amount);
	for (size_t i = 0; i < amount; i++)
	{
		glm::vec3 particleDirection = glm::normalize(glm::vec3(Random::RandomFloat(-100, 100), Random::RandomFloat(-100, 100), Random::RandomFloat(-100, 100)));
		float speed = Random::RandomFloat(1, 1);
		particles.emplace_back(std::make_unique<Particle>(particleDirection, speed));
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

glm::vec4 ParticleSystem::GetColor() const
{
	return color;
}

glm::vec3 ParticleSystem::GetPosition() const
{
	return position;
}
