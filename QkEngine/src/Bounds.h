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

	Bounds& operator = (const Bounds& other);

	void SetExtents(const glm::vec3& newExtents);
	glm::vec3 Extents() const;
	glm::vec3 ExtentsHalf() const;
	bool Intersects(const glm::vec3& point, const glm::vec3& offset) const;

private:
	glm::vec3 extents = glm::vec3(0, 0, 0);
};