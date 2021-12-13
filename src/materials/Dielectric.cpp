#include "Dielectric.h"

Dielectric::Dielectric(double ri)
{
    ref_idx = ri;
}

bool Dielectric::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)
{
    Vec3 outward_normal;
    Vec3 reflected = Reflect(rayIn.getDirection(), hitRecord.normal);
    double ni_over_nt;

    attenuation = Vec3(1.0, 1.0, 1.0);
    Vec3 refracted;
    double reflectProbability;
    double cosine;

    if (Vec3::dot(rayIn.getDirection(), hitRecord.normal) > 0)
    {
        outward_normal = -hitRecord.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * Vec3::dot(rayIn.getDirection(), hitRecord.normal) / rayIn.getDirection().length();
    }
    else
    {
        outward_normal = hitRecord.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -Vec3::dot(rayIn.getDirection(), hitRecord.normal) / rayIn.getDirection().length();
    }

    if (Refract(rayIn.getDirection(), outward_normal, ni_over_nt, refracted))
    {
        reflectProbability = Schlick(cosine, ref_idx);
    }
    else
    {
        reflectProbability = 1.0;
    }

    if (getRandom() < reflectProbability)
    {
        // TODO: Add roughness factor
        //scattered = Ray(hitRecord.p, reflected * randomDirection());
        scattered = Ray(hitRecord.p, reflected);
    }
    else
    {
        scattered = Ray(hitRecord.p, refracted);
    }

    return true;
}

Vec3 Dielectric::Reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * Vec3::dot(v, n) * n;
}

bool Dielectric::Refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted)
{
    Vec3 uv = Vec3::unit_vector(v);
    double dt = Vec3::dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

double Dielectric::Schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

Vec3 Dielectric::getAlbedo()
{
    return Vec3(0.1, 0.1, 0.1);
}

Vec3 Dielectric::randomDirection()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(getRandom(), getRandom(), getRandom()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}