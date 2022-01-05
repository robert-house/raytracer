#pragma once
#include "../Ray.h"
#include "../HitRecord.h"
#include <random>
#include <glm/glm.hpp>

enum MaterialType
{
    DIELECTRIC,
    METAL,
    LAMBERTIAN,
    DIFFUSE_EMITTER,
    MISSING
};

class Material
{
public:
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)  = 0;
    virtual glm::vec3 emittRay(double u, double v,  glm::vec3& p)  { return glm::vec3(0.0, 0.0, 0.0); };
    virtual glm::vec3 getAlbedo() = 0;

protected:
    float getRandom() ; // TODO: Change to new fast hash random algo and change seed in Random.cpp
};