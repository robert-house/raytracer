#pragma once
#include <string>
#include <sstream>
#include "Shape.h"
#include "../HitRecord.h"

#include <glm/glm.hpp>


constexpr float kEpsilon = 1e-8;

class Triangle : public Shape
{
public:
    Triangle(glm::vec3 vertA, glm::vec3 vertB, glm::vec3 vertC, float size, Material* pMaterial);

    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
    virtual std::string toString();

    glm::vec3 _v0;
    glm::vec3 _v1;
    glm::vec3 _v2;
    float _size;
    glm::vec3 _normal;

private:
};