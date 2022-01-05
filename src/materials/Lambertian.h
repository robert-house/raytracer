#pragma once
#include "Material.h"
#include <random>
#include "../Random.h"

class Lambertian : public Material
{
public:
    Lambertian(double albedoR, double albedoG, double albedoB);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) ;
    virtual glm::vec3 getAlbedo();

private:
    glm::vec3 _albedo;
    glm::vec3 randomDirection() ;
     Random random;
};