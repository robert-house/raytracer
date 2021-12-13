#pragma once
#include "Ray.h"

class Material;

class HitRecord
{
public:
    HitRecord();

    double u;
    double v;
    double t;
    Vec3 p;
    Vec3 normal;
    Material* pMaterial;
};