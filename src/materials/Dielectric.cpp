#include "Dielectric.h"

Dielectric::Dielectric(float ri)
{
    ref_idx = ri;
}

bool Dielectric::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered)
{
    glm::vec3 outward_normal;
    glm::vec3 reflected = Reflect(rayIn.getDirection(), hitRecord.normal);
    double ni_over_nt;

    attenuation = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 refracted;
    double reflectProbability;
    double cosine;

    if (glm::dot(rayIn.getDirection(), hitRecord.normal) > 0)
    {
        outward_normal = -hitRecord.normal;
        ni_over_nt = ref_idx;
        cosine = ref_idx * glm::dot(rayIn.getDirection(), hitRecord.normal) / rayIn.getDirection().length();
    }
    else
    {
        outward_normal = hitRecord.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -glm::dot(rayIn.getDirection(), hitRecord.normal) / rayIn.getDirection().length();
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

glm::vec3 Dielectric::Reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - 2 * glm::dot(v, n) * n;
}

bool Dielectric::Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
    glm::vec3 uv = glm::normalize(v);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * (float)(sqrt(discriminant));
        return true;
    }
    else
    {
        return false;
    }
}

float Dielectric::Schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

glm::vec3 Dielectric::getAlbedo()
{
    return glm::vec3(0.1, 0.1, 0.1);
}

glm::vec3 Dielectric::randomDirection()
{
    glm::vec3 p;
    do
    {
        p = 2.0f * (glm::vec3(getRandom(), getRandom(), getRandom()) - glm::vec3(1, 1, 1));
    } while (p.x *p.x + p.y * p.y + p.z * p.z >= 1.0);

    return p;
}