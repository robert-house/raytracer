#include "MissingMat.h"

MissingMat::MissingMat()
{
    _albedo.e[0] = 1.0;
    _albedo.e[1] = 0.078;
    _albedo.e[2] = 0.576;
}

MissingMat::~MissingMat() {}

bool MissingMat::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)
{
    attenuation = _albedo;
    Vec3 target = hitRecord.p + hitRecord.normal;
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    return true;
}

Vec3 MissingMat::emittRay(double u, double v,  Vec3& p)
{
    return _albedo;
}

Vec3 MissingMat::getAlbedo() { return _albedo; }