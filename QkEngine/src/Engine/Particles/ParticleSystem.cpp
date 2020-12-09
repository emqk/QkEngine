#include "ParticleSystem.h"
#include "../Random.h"

ParticleSystem::ParticleSystem(const glm::vec3& pos, const glm::vec3& _scale, Texture* tex, Mesh* mesh, Shader* shader, const glm::vec4& _color, const int& amount)
	: position(pos), scale(_scale), particleTexture(tex), particleMesh(mesh), particleShader(shader), color(_color)
{
	std::uniform_real_distribution<float> directionDistr(-100, 100);
	particles.reserve(amount);
	for (size_t i = 0; i < amount; i++)
	{
		glm::vec3 particleDirection = glm::normalize(glm::vec3(Random::RandomFloat(directionDistr), Random::RandomFloat(directionDistr), Random::RandomFloat(directionDistr)));
		particles.emplace_back(std::make_unique<Particle>(particleDirection, 1));
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

	scale -= deltaTime / 2.0f;
	color.a -= deltaTime / 2.0f;

	if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0 || color.a <= 0)
	{
		shouldBeDead = true;
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

glm::vec3 ParticleSystem::GetScale() const
{
	return scale;
}

bool ParticleSystem::ShouldBeDead() const
{
	return shouldBeDead;
}
