#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const glm::vec3& a, const glm::vec3& b)
{
    A = a;
    B = b;
}

Ray::~Ray() {}

glm::vec3 Ray::getOrigin() const
{
    return A;
}

glm::vec3 Ray::getDirection() const
{
    return B;
}

glm::vec3 Ray::pointAtParameter(float t) const
{
    return A + (t * B);
}