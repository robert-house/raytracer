#include "DiffuseEmitter.h"

DiffuseEmitter::DiffuseEmitter(double r, double g, double b)
{
    _albedo = Vec3(r,g,b);
}

DiffuseEmitter::~DiffuseEmitter()
{

}

bool DiffuseEmitter::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)
{
    return false;
}

Vec3 DiffuseEmitter::getAlbedo()
{
    return _albedo;
}

Vec3 DiffuseEmitter::emittRay(double u, double v,  Vec3& p)
{
    // Simple emitting material that emits its albedo
    return _albedo;
}