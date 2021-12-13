#include "Metal.h"

Metal::Metal (double albedoR, double albedoG, double albedoB, double roughness)
{
    _albedo = Vec3(albedoR, albedoG, albedoB);

    if (roughness < 1)
    {
        _roughness = roughness;
    }
    else
    {
        // Clamp to one
        _roughness = 1;
    }
}

bool Metal::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)
{
    Vec3 reflected = Reflect(Vec3::unit_vector(rayIn.getDirection()), hitRecord.normal);
    scattered = Ray(hitRecord.p, reflected + _roughness * randomDirection());
    attenuation = _albedo;
    return (Vec3::dot(scattered.getDirection(), hitRecord.normal) > 0);
}

Vec3 Metal::getAlbedo()
{
    return _albedo;
}

Vec3 Metal::randomDirection()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(getRandom(), getRandom(), getRandom()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}

Vec3 Metal::Reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * Vec3::dot(v, n) * n;
}