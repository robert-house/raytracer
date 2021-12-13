#pragma once

#include "Material.h"

class DiffuseEmitter : public Material
{
public:
    DiffuseEmitter(double r, double g, double b);
    ~DiffuseEmitter();

    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered) ;
    virtual Vec3 emittRay(double u, double v,  Vec3& p) ;
    virtual Vec3 getAlbedo();

private:
    Vec3 _albedo;
};