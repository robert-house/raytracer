#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const Vec3& a, const Vec3& b)
{
    A = a;
    B = b;
}

Ray::~Ray() {}

Vec3 Ray::getOrigin() const
{
    return A;
}

Vec3 Ray::getDirection() const
{
    return B;
}

Vec3 Ray::pointAtParameter(double t) const
{
    //return A + t * B;

    double bx = B.e[0] * t;
    double by = B.e[1] * t;
    double bz = B.e[2] * t;

    return Vec3(A.e[0] + bx, A.e[1] + by, A.e[2] + bz);
}