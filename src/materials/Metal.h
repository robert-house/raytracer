#pragma once
#include "Material.h"
#include <random>

class Metal : public Material
{
public:
    Metal(double albedoR, double albedoG, double albedoB, double roughness);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered) ;
    virtual Vec3 getAlbedo();

private:
    Vec3 randomDirection() ;
    Vec3 Reflect(const Vec3& v, const Vec3& n) ;

    double _roughness;
    Vec3 _albedo;
};