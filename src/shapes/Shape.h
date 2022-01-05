#pragma once

#include "../Ray.h"
#include "../HitRecord.h"
#include "../materials/Material.h"

class Shape
{
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord) = 0;
    virtual bool UpdateMaterial(Material* pMaterial) { _pMaterial = pMaterial; return true; }
    virtual std::string toString() = 0;

protected:
    Material* _pMaterial;
};