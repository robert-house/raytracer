//=========================================================================//
// MissingMat.h                                                            //
//=========================================================================//
// Missing material shader                                                 //
// Robert House                                                            //
// 16 FEB 2020                                                             //
//-------------------------------------------------------------------------//
// MissingMat: Material that stands out when an unknown material type is   //
//             loaded from disk. Obnoxious hot pink color for fun          //
//-------------------------------------------------------------------------//
#pragma once
#include "Material.h"
#include <glm/glm.hpp>

class MissingMat : public Material
{
public:
    MissingMat();
    ~MissingMat();

    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) ;
    virtual glm::vec3 emittRay(double u, double v,  glm::vec3& p) ;
    virtual glm::vec3 getAlbedo();

private:
    glm::vec3 _albedo;
};