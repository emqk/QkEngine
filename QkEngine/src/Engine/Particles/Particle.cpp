#include "Particle.h"

Particle::Particle(const glm::vec3& dir, const float& moveSpeed, const glm::vec4& _color) 
	: direction(dir), speed(moveSpeed), color(_color), position(glm::vec3(0,0,0))
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

glm::vec4 Particle::GetColor()
{
	return color;
}

float Particle::GetSpeed()
{
	return speed;
}
