#pragma once
#include "Shape.h"
#include <algorithm>
#include <glm/glm.hpp>

class Rect : public Shape
{
public:
    enum RectType
    {
        XY,
        XZ,
        YZ
    };

public:
    Rect(float x0, float x1, float y0, float y1, float nz, float size, Material* pMaterial);
    Rect(RectType rectType, glm::vec3 pos, float length, float width, int normalDirection, Material* pMaterial);
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
    virtual std::string toString();


    RectType _rectType;
    glm::vec3 _position;
    float _length;
    float _width;
    int _normalDirection;

private:
    float _x0, _x1, _y0, _y1, _z0, _z1, _nz, _size;
    bool _useOldHit;
    float _dim1_0, _dim1_1, _dim2_0, _dim2_1, _depth;
    glm::vec3 _normal;
    bool oldHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
    bool newHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
    bool newFastHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord);
};