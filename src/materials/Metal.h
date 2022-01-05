#pragma once
#include "Material.h"
#include <random>
#include <glm/glm.hpp>

class Metal : public Material
{
public:
    Metal(float albedoR, float albedoG, float albedoB, float roughness);
    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) ;
    virtual glm::vec3 getAlbedo();

private:
    glm::vec3 randomDirection() ;
    glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n) ;

    float _roughness;
    glm::vec3 _albedo;
};