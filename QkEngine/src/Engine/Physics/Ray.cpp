#include "Ray.h"

Ray::Ray(const glm::vec3& orig, const glm::vec3& dir) : origin(orig), direction(dir)
{
}

Ray::~Ray()
{
}

glm::vec3 Ray::GetOrigin() const
{
	return origin;
}

glm::vec3 Ray::GetDirection() const
{
	return direction;
}
