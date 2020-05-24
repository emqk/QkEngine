#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>

class Bounds
{
public:
	Bounds();
	Bounds(const float& _minX, const float& _maxX, const float& _minY, const float& _maxY, const float& _minZ, const float& _maxZ);
	~Bounds();
	Bounds(const Bounds& otherBounds);

	friend Bounds operator + (const Bounds& c1, const Bounds& c2);
	Bounds& operator = (const Bounds& other);

	glm::vec3 Extents() const;
	bool Intersects(const glm::vec3& point, const glm::vec3& offset) const;

private:
	glm::vec3 extents;
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
};