#pragma once
#include "Material.h"
#include <random>
#include "../Random.h"

class Lambertian : public Material
{
public:
    Lambertian(double albedoR, double albedoG, double albedoB);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered) ;
    virtual Vec3 getAlbedo();

private:
    Vec3 _albedo;
    Vec3 randomDirection() ;
     Random random;
};