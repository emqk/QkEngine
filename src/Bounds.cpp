#include "Bounds.h"
#include <algorithm>
#include <iostream>

Bounds::Bounds()
{
}

Bounds::Bounds(const float& _minX, const float& _maxX, const float& _minY, const float& _maxY, const float& _minZ, const float& _maxZ) :
                minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY), minZ(_minZ), maxZ(_maxZ)
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

Bounds operator+(const Bounds& c1, const Bounds& c2)
{
    return Bounds(c1.minX + c2.minX, c1.maxX + c2.maxX, c1.minY + c2.minY, c1.maxY + c2.maxY, c1.minZ + c2.minZ, c1.maxZ + c2.maxZ);
}

Bounds& Bounds::operator=(const Bounds& other)
{
    extents = other.extents;

    minX = other.minX;
    maxX = other.maxX;

    minY = other.minY;
    maxY = other.maxY;

    minZ = other.minZ;
    maxZ = other.maxZ;

    return *this;
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