#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float size, Material* pMaterial)
{
    _v0 = v0 * size;
    _v1 = v1 * size;
    _v2 = v2 * size;
    _size = size;
    _pMaterial = pMaterial;

    // Calculate Normal Vector
    A = _v1 - _v0;
    B = _v2 - _v0;

    _normal = glm::cross((_v1 - _v0),(_v2 - _v0));
    _normal = -glm::normalize(_normal);
}

// t : Distance
// A = _v0 - _v1;
// B = _v2 - _v0;
bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord)
{
    glm::vec3 pvec = glm::cross(ray.getDirection(), B);
    float determinant = glm::dot(pvec, A);

    if (fabs(determinant) < kEpsilon) return false;

    float invDeterminant = 1 / determinant;

    glm::vec3 tvec = ray.getOrigin() - _v0;
    float u = glm::dot(tvec,pvec) * invDeterminant;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, A);
    float v = glm::dot(ray.getDirection(), qvec) * invDeterminant;
    if (v < 0 || u + v > 1) return false;

    float t = glm::dot(B, qvec) * invDeterminant;
    if (t < tMin || t > tMax) return false;

    // Detected a hit, fill out hitrecord
    hitRecord.normal = _normal;
    hitRecord.u = u;
    hitRecord.v = v;
    hitRecord.t = t;
    hitRecord.p = ray.pointAtParameter(t);
    hitRecord.pMaterial = _pMaterial;

    return true;
}

std::string Triangle::toString()
{
    std::ostringstream ss;
    // TODO: Add other 2 dims
    ss << "Vertices: " << std::to_string(_v0.x) << ","
                       << std::to_string(_v1.x) << ","
                       << std::to_string(_v2.x)
       << "Size: " << _size;

    return ss.str();
}