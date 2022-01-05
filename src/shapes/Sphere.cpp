#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float r, Material* pMaterial)
{
    _center = center;
    _radius = r;
    _pMaterial = pMaterial;
}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord)
{
    glm::vec3 oc = ray.getOrigin() - _center;

    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - _radius * _radius;
    float discriminant = b * b - a * c;

    // Assign Material
    hitRecord.pMaterial = _pMaterial;

    if (discriminant > 0)
    {
        float root1 = (-b - sqrt(b * b - a * c)) / a;
        float root2 = (-b + sqrt(b * b - a * c)) / a;

        // If either of the two roots intersect with this sphere...
        if ((root1 < tMax && root1 > tMin))
        {
            hitRecord.t = root1;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - _center) / _radius;
            return true;
        }
        if ((root2 < tMax && root2 > tMin))
        {
            hitRecord.t = root2;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - _center) / _radius;
            return true;
        }
    }
    return false;
}

std::string Sphere::toString()
{
    std::ostringstream ss;
    ss << "[SPHERE] Position: " << std::to_string(_center.x) // TODO: fix me!
       << " Radius: " << (int)_radius;

    return ss.str();
}