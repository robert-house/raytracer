#pragma once
#include "Ray.h"
#include <glm/glm.hpp>

class Material;

class HitRecord
{
public:
    HitRecord();

    double u;
    double v;
    double t;
    glm::vec3 p;
    glm::vec3 normal;
    Material* pMaterial;
};