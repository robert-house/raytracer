#pragma once
#include "Shape.h"
#include "../HitRecord.h"
#include "../materials/Material.h"
#include <sstream>

class Sphere : public Shape
{
public:
    Sphere(glm::vec3 center, float r, Material* pMaterial);

    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
    virtual std::string toString();

    glm::vec3 _center;
    float _radius;

private:
};