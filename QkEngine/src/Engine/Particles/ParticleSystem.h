#pragma once
#include "../Texture.h"
#include "../Mesh.h"
#include "Particle.h"

#include <memory>


class ParticleSystem
{
public:
	ParticleSystem(Texture* tex, Mesh* mesh, Shader* shader, const int& amount);
	~ParticleSystem();

	ParticleSystem(const ParticleSystem& particleSystem) = delete;
	ParticleSystem& operator = (const ParticleSystem& particleSystem) = delete;

	ParticleSystem(ParticleSystem&& particleSystem) = delete;
	ParticleSystem& operator = (ParticleSystem&& particleSystem) = delete;

	void Update(const float& deltaTime);

	const std::vector<std::unique_ptr<Particle>>& GetParticles() const;
	Texture* GetTexture() const;
	Mesh* GetMesh() const;
	Shader* GetShader() const;

private:

	std::vector<std::unique_ptr<Particle>> particles;

	Texture* particleTexture;
	Mesh* particleMesh;
	Shader* particleShader;
};