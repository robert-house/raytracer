#include "Triangle.h"

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2, double size, Material* pMaterial)
{
    _v0 = v0;
    _v1 = v1;
    _v2 = v2;
    _size = size;
    _pMaterial = pMaterial;
}

bool Triangle::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord)
{
    // Calculate the normal
    Vec3 normal = Vec3::cross(_v1 - _v0, _v2 - _v0);
    double denominator = Vec3::dot(normal, normal);
    double NdotRayDirection = Vec3::dot(normal, ray.getDirection());

    // Test is Ray is parallel
    if (abs(NdotRayDirection) < kEpsilon)
    {
        return false;
    }

    // Find P

    double d = Vec3::dot(normal, _v0);
    double t = (Vec3::dot(normal, ray.getOrigin()) + d) / NdotRayDirection;


    Vec3 P = ray.pointAtParameter(t);

    Vec3 C;

    // Test to see if triangle lies within
    Vec3 edge0 = _v1 - _v0;
    Vec3 vp0 = P - _v0;
    C = Vec3::cross(edge0, vp0);
    if (Vec3::dot(normal, C) < 0)
    {
        return false;
    }

    Vec3 edge1 = _v2 - _v1;
    Vec3 vp1 = P - _v1;
    C = Vec3::cross(edge1, vp1);
    if (Vec3::dot(normal, C) < 0)
    {
        return false;
    }

    Vec3 edge2 = _v0 - _v2;
    Vec3 vp2 = P - _v2;
    C = Vec3::cross(edge2, vp2);
    if (Vec3::dot(normal, C) < 0)
    {
        return false;
    }

    // Detected a hit, fill out hitrecord
    hitRecord.normal = normal;
    hitRecord.p = P;
    hitRecord.t = t;
    hitRecord.pMaterial = _pMaterial;

    return true;
}

std::string Triangle::toString()
{
    std::ostringstream ss;

    ss << "Vertices: " << _v0.toString() << ","
                       << _v1.toString() << ","
                       << _v2.toString()
       << "Size: " << _size;

    return ss.str();
}