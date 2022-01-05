#include "Metal.h"

Metal::Metal (float albedoR, float albedoG, float albedoB, float roughness)
{
    _albedo = glm::vec3(albedoR, albedoG, albedoB);

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

bool Metal::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    glm::vec3 reflected = Reflect(glm::normalize(rayIn.getDirection()), hitRecord.normal);
    scattered = Ray(hitRecord.p, reflected + _roughness * randomDirection());
    attenuation = _albedo;
    return (glm::dot(scattered.getDirection(), hitRecord.normal) > 0);
}

glm::vec3 Metal::getAlbedo()
{
    return _albedo;
}

glm::vec3 Metal::randomDirection()
{
    glm::vec3 p;
    do
    {
        p = 2.0f * glm::vec3(getRandom(), getRandom(), getRandom()) - glm::vec3(1, 1, 1);
    } while (p.x *p.x + p.y * p.y + p.z * p.z >= 1.0);

    return p;
}

glm::vec3 Metal::Reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - 2 * glm::dot(v, n) * n;
}