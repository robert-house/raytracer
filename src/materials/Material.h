#pragma once
#include "../Ray.h"
#include "../HitRecord.h"
#include <random>

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
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)  = 0;
    virtual Vec3 emittRay(double u, double v,  Vec3& p)  { return Vec3(0.0, 0.0, 0.0); };
    virtual Vec3 getAlbedo() = 0;

protected:
    double getRandom() ;
};