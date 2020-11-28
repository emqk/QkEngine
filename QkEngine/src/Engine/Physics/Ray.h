#pragma once
#include <glm\ext\vector_float3.hpp>

class Ray
{
public:
    Ray(const glm::vec3& orig, const glm::vec3& dir);
    ~Ray();

    glm::vec3 GetOrigin() const;
    glm::vec3 GetDirection() const;

private:
    glm::vec3 origin;
    glm::vec3 direction;
};
