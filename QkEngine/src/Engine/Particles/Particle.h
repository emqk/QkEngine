#pragma once
#include <glm\ext\vector_float3.hpp>

class Particle
{
public:
	Particle(const glm::vec3& dir, const float& moveSpeed);
	~Particle();

	Particle(const Particle& particle) = delete;
	Particle& operator = (const Particle& particle) = delete;

	Particle(Particle&& particle) = delete;
	Particle& operator = (Particle&& particle) = delete;

	void Update(const float& deltaTime);

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	float GetSpeed();

private:
	glm::vec3 position;
	glm::vec3 direction;
	float speed;
};