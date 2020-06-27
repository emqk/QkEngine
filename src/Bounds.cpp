#include "Bounds.h"
#include <algorithm>
#include <iostream>

Bounds::Bounds()
{
}

Bounds::Bounds(const float& _minX, const float& _maxX, const float& _minY, const float& _maxY, const float& _minZ, const float& _maxZ)
{
	extents = glm::vec3(std::max(_minX, _maxX), std::max(_minY, _maxY), std::max(_minZ, _maxZ));
}

Bounds::~Bounds()
{
}

Bounds::Bounds(const Bounds& otherBounds)
{
    *this = otherBounds;
}

Bounds& Bounds::operator=(const Bounds& other)
{
    extents = other.extents;

    return *this;
}

void Bounds::SetExtents(const glm::vec3& newExtents)
{
    extents = newExtents;
}

glm::vec3 Bounds::Extents() const
{
	return extents;
}

bool Bounds::Intersects(const glm::vec3& point, const glm::vec3& offset) const
{
    if (   extents.x + offset.x >= point.x && offset.x - extents.x <= point.x
        && extents.y + offset.y >= point.y && offset.y - extents.y <= point.y
        && extents.z + offset.z >= point.z && offset.z - extents.z <= point.z)
    {
        return true;
    }

	return false;
}