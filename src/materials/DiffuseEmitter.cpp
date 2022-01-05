#include "DiffuseEmitter.h"

DiffuseEmitter::DiffuseEmitter(double r, double g, double b)
{
    _albedo = glm::vec3(r,g,b);
}

DiffuseEmitter::~DiffuseEmitter()
{

}

bool DiffuseEmitter::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    return false;
}

glm::vec3 DiffuseEmitter::getAlbedo()
{
    return _albedo;
}

glm::vec3 DiffuseEmitter::emittRay(double u, double v,  glm::vec3& p)
{
    // Simple emitting material that emits its albedo
    return _albedo;
}