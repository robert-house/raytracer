#pragma once
#include "Vec3.h"


class Ray
{
public:
    Ray();
    Ray(const Vec3& a, const Vec3& b);
    ~Ray();

    Vec3 getOrigin() const;
    Vec3 getDirection() const;
    Vec3 pointAtParameter(double t) const;

private:
    Vec3 A;
    Vec3 B;
};