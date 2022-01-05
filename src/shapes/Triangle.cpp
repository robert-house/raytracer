#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float size, Material* pMaterial)
{
    _v0 = v0;
    _v1 = v1;
    _v2 = v2;
    _size = size;
    _pMaterial = pMaterial;
}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord)
{
    glm::vec3 v0v1 = _v0 - _v1;
    glm::vec3 v0v2 = _v2 - _v0;
    glm::vec3 pvec = glm::cross(ray.getDirection(), v0v2);
    float determinant = glm::dot(pvec, v0v1);

    if (fabs(determinant) < kEpsilon) return false;

    float invDeterminant = 1 / determinant;

    glm::vec3 tvec = ray.getOrigin() - _v0;

    float u = glm::dot(tvec,pvec) * invDeterminant;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    float v = glm::dot(ray.getDirection(), pvec) * invDeterminant;
    if (v < 0 || u + v > 1) return false;

    // Detected a hit, fill out hitrecord
    hitRecord.normal = _normal;
    hitRecord.u = u;
    hitRecord.v = v;
    hitRecord.t = glm::dot(v0v2, qvec) * invDeterminant;
    hitRecord.p = ray.pointAtParameter(hitRecord.t);
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