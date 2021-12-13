#pragma once
#include <string>
#include "Shape.h"
#include "../HitRecord.h"


constexpr float kEpsilon = 1e-8;

class Triangle : public Shape
{
public:
    Triangle(Vec3 vertA, Vec3 vertB, Vec3 vertC, double size, Material* pMaterial);

    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
    virtual std::string toString();

    Vec3 _v0;
    Vec3 _v1;
    Vec3 _v2;
    double _size;

private:
};