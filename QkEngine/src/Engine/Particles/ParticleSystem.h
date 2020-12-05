#pragma once
#include "../Texture.h"
#include "../Mesh.h"
#include "Particle.h"

#include <memory>


class ParticleSystem
{
public:
	ParticleSystem(const glm::vec3& pos, Texture* tex, Mesh* mesh, Shader* shader, const glm::vec4& _color, const int& amount);
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
	glm::vec4 GetColor() const;
	glm::vec3 GetPosition() const;

private:

	std::vector<std::unique_ptr<Particle>> particles;

	Texture* particleTexture;
	Mesh* particleMesh;
	Shader* particleShader;
	glm::vec4 color;
	glm::vec3 position;
};