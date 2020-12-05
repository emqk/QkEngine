#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float4.hpp>

class Particle
{
public:
	Particle(const glm::vec3& dir, const float& moveSpeed, const glm::vec4& _color);
	~Particle();

	Particle(const Particle& particle) = delete;
	Particle& operator = (const Particle& particle) = delete;

	Particle(Particle&& particle) = delete;
	Particle& operator = (Particle&& particle) = delete;

	void Update(const float& deltaTime);

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	glm::vec4 GetColor();
	float GetSpeed();

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec4 color;
	float speed;
};