#pragma once
#include "Material.h"
#include <random>
#include <glm/glm.hpp>

class Dielectric : public Material
{
public:
    Dielectric(float ri);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) ;
    virtual glm::vec3 getAlbedo();
    float ref_idx;

    glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n) ;
    bool Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted) ;
    float Schlick(float cosine, float ref_idx) ;
    glm::vec3 randomDirection() ;
};