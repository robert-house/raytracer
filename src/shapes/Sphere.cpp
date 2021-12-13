#include "Sphere.h"

Sphere::Sphere(Vec3 center, double r, Material* pMaterial)
{
    _center = center;
    _radius = r;
    _pMaterial = pMaterial;
}

bool Sphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord)
{
    Vec3 oc = ray.getOrigin() - _center;

    double a = Vec3::dot(ray.getDirection(), ray.getDirection());
    double b = Vec3::dot(oc, ray.getDirection());
    double c = Vec3::dot(oc, oc) - _radius * _radius;
    double discriminant = b * b - a * c;

    // Assign Material
    hitRecord.pMaterial = _pMaterial;

    if (discriminant > 0)
    {
        double root1 = (-b - sqrt(b * b - a * c)) / a;
        double root2 = (-b + sqrt(b * b - a * c)) / a;

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
    ss << "[SPHERE] Position: " << _center.toString()
       << " Radius: " << (int)_radius;

    return ss.str();
}