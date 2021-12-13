#pragma once
#include "Shape.h"

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
    Rect(double x0, double x1, double y0, double y1, double nz, double size, Material* pMaterial);
    Rect(RectType rectType, Vec3 pos, double length, double width, int normalDirection, Material* pMaterial);
    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
    virtual std::string toString();


    RectType _rectType;
    Vec3 _position;
    double _length;
    double _width;
    int _normalDirection;

private:
    double _x0, _x1, _y0, _y1, _z0, _z1, _nz, _size;
    bool _useOldHit;
    double _dim1_0, _dim1_1, _dim2_0, _dim2_1, _depth;
    Vec3 _normal;
    bool oldHit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
    bool newHit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
    bool newFastHit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord);
};