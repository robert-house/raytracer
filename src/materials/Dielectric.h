#pragma once
#include "Material.h"
#include <random>

class Dielectric : public Material
{
public:
    Dielectric(double ri);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered) ;
    virtual Vec3 getAlbedo();
    double ref_idx;

    Vec3 Reflect(const Vec3& v, const Vec3& n) ;
    bool Refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted) ;
    double Schlick(double cosine, double ref_idx) ;
    Vec3 randomDirection() ;
};