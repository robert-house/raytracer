#include "MissingMat.h"

MissingMat::MissingMat()
{
    _albedo.x = 1.0;
    _albedo.y = 0.078;
    _albedo.z = 0.576;
}

MissingMat::~MissingMat() {}

bool MissingMat::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    attenuation = _albedo;
    glm::vec3 target = hitRecord.p + hitRecord.normal;
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    return true;
}

glm::vec3 MissingMat::emittRay(double u, double v,  glm::vec3& p)
{
    return _albedo;
}

glm::vec3 MissingMat::getAlbedo() { return _albedo; }