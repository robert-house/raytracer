#pragma once

#include "Material.h"

class DiffuseEmitter : public Material
{
public:
    DiffuseEmitter(double r, double g, double b);
    ~DiffuseEmitter();

    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) ;
    virtual glm::vec3 emittRay(double u, double v,  glm::vec3& p) ;
    virtual glm::vec3 getAlbedo();

private:
    glm::vec3 _albedo;
};