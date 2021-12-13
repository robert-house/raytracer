#pragma once
#include "Shape.h"
#include "../HitRecord.h"
#include "../materials/Material.h"

class Sphere : public Shape
{
public:
    Sphere(Vec3 center, double r, Material* pMaterial);

    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
    virtual std::string toString();

    Vec3 _center;
    double _radius;

private:
};