#include "Particle.h"

Particle::Particle(const glm::vec3& dir, const float& moveSpeed) 
	: direction(dir), speed(moveSpeed), position(glm::vec3(0,0,0))
{
}

Particle::~Particle()
{
}

void Particle::Update(const float& deltaTime)
{
	position += direction * speed * deltaTime;
}

glm::vec3 Particle::GetPosition()
{
	return position;
}

glm::vec3 Particle::GetDirection()
{
	return direction;
}

float Particle::GetSpeed()
{
	return speed;
}
