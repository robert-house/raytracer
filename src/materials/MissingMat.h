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

class MissingMat : public Material
{
public:
    MissingMat();
    ~MissingMat();

    virtual bool scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered) ;
    virtual Vec3 emittRay(double u, double v,  Vec3& p) ;
    virtual Vec3 getAlbedo();

private:
    Vec3 _albedo;
};