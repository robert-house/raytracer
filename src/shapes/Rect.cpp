#include "Rect.h"

Rect::Rect(float x0, float x1, float y0, float y1, float nz, float size, Material* pMaterial)
{
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
    _nz = nz;
    _size = size;
    _pMaterial = pMaterial;
    _rectType = RectType::XY;
    _useOldHit = true;
}

Rect::Rect(RectType rectType, glm::vec3 pos, float length, float width, int normalDirection, Material* pMaterial)
{
    _rectType = rectType;
    _position = pos;
    _length = length;
    _width = width;
    _normalDirection = normalDirection;
    _pMaterial = pMaterial;
    _size = 0.0;
    _useOldHit = false;

    // Compute local variables
    // Trade loadtime for runtime performance
    switch (_rectType)
    {
    case XY:

        _x0 = pos.x - (width / 2);
        _x1 = pos.x + (width / 2);
        _y0 = pos.y - (length / 2);
        _y1 = pos.y + (length / 2);
        _dim1_0 = pos.x - (width / 2);
        _dim1_1 = pos.x + (width / 2);
        _dim2_0 = pos.y - (length / 2);
        _dim2_1 = pos.y + (length / 2);
        _depth = pos.z;
        if (normalDirection < 0)
        {
            _normal = glm::vec3(0, 0, -1);
        }
        else
        {
            _normal = glm::vec3(0, 0, 1);
        }

        break;
    case XZ:
        _x0 = pos.x - (length / 2);
        _x1 = pos.x + (length / 2);
        _z0 = pos.z - (width / 2);
        _z1 = pos.z + (width / 2);
        _dim1_0 = pos.x - (length / 2);
        _dim1_1 = pos.x + (length / 2);
        _dim2_0 = pos.z - (width / 2);
        _dim2_1 = pos.z + (width / 2);
        _depth = pos.y;
        if (normalDirection < 0)
        {
            _normal = glm::vec3(0, -1, 0);
        }
        else
        {
            _normal = glm::vec3(0, 1, 0);
        }
        break;
    case YZ:
        _y0 = pos.y - (width / 2);
        _y1 = pos.y + (width / 2);
        _z0 = pos.z - (length / 2);
        _z1 = pos.z + (length / 2);
        _dim1_0 = pos.y - (width / 2);
        _dim1_1 = pos.y + (width / 2);
        _dim2_0 = pos.z - (length / 2);
        _dim2_1 = pos.z + (length / 2);
        _depth = pos.x;
        if (normalDirection < 0)
        {
            _normal = glm::vec3(-1, 0, 0);
        }
        else
        {
            _normal = glm::vec3(1, 0, 0);
        }
        break;
    }
}

bool Rect::hit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord)
{
    //return oldHit(r, tMin, tMax, hitRecord);
    return newFastHit(r, tMin, tMax, hitRecord);
}

bool Rect::oldHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord)
{
    float t = (_size - r.getOrigin().z) / r.getDirection().z;

    if (t < tMin || t > tMax)
    {
        return false;
    }

    float x = r.getOrigin().x + t * r.getDirection().x;
    float y = r.getOrigin().y + t * r.getDirection().y;

    if (x < _x0 || x > _x1 || y < _y0 || y > _y1)
    {
        return false;
    }

    hitRecord.u = (x - _x0) / (_x1 - _x0);
    hitRecord.u = (y - _y0) / (_y1 - _y0);
    hitRecord.t = t;
    hitRecord.pMaterial = _pMaterial;
    hitRecord.p = r.pointAtParameter(t);
    hitRecord.normal = glm::vec3(0, 0, 1);

    if (_nz == -1)
    {
        hitRecord.normal = -hitRecord.normal;
    }

    return true;
}

// Keeping this here as an example of how a good idea ends up doubling runtime cost.
// I think the nested conditionals really kill performance.
// TODO: More research on this topic
bool Rect::newHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord)
{
    float t = 0.0;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    switch (_rectType)
    {
    case XY:
        t = (_position.z - r.getOrigin().z) / r.getDirection().z;
        if (t < tMin || t > tMax)
        {
            return false;
        }
        x = r.getOrigin().x + t * r.getDirection().x;
        y = r.getOrigin().y + t * r.getDirection().y;
        if (x < _x0 || x > _x1 || y < _y0 || y > _y1)
        {
            return false;
        }
        hitRecord.u = (x - _x0) / (_x1 - _x0);
        hitRecord.v = (y - _y0) / (_y1 - _y0);
        hitRecord.normal = glm::vec3(0, 0, 1);
        break;
    case XZ:
        t = (_position.y - r.getOrigin().y) / r.getDirection().y;
        if (t < tMin || t > tMax)
        {
            return false;
        }
        x = r.getOrigin().x + t * r.getDirection().x;
        z = r.getOrigin().z + t * r.getDirection().z;
        if (x < _x0 || x > _x1 || z < _z0 || z > _z1)
        {
            return false;
        }
        hitRecord.u = (x - _x0) / (_x1 - _x0);
        hitRecord.v = (z - _z0) / (_z1 - _z0);
        hitRecord.normal = glm::vec3(0, 1, 0);
        break;
    case YZ:
        t = (_position.x - r.getOrigin().x) / r.getDirection().x;
        if (t < tMin || t > tMax)
        {
            return false;
        }
        y = r.getOrigin().y + t * r.getDirection().y;
        z = r.getOrigin().z + t * r.getDirection().z;
        if (y < _y0 || y > _y1 || z < _z0 || z > _z1)
        {
            return false;
        }
        hitRecord.u = (y - _y0) / (_y1 - _y0);
        hitRecord.v = (z - _z0) / (_z1 - _z0);
        hitRecord.normal = glm::vec3(1, 0, 0);
        break;
    }

    hitRecord.t = t;
    hitRecord.pMaterial = _pMaterial;
    hitRecord.p = r.pointAtParameter(t);

    if (_normalDirection == -1)
    {
        hitRecord.normal = -hitRecord.normal;
    }

    return true;
}

bool Rect::newFastHit(const Ray& r, float tMin, float tMax, HitRecord& hitRecord)
{
    float t = 0.0;
    float dim1 = 0.0;
    float dim2 = 0.0;

    // I don't think I can get around the if statements here :(
    if (_rectType == XY)
    {
        t = (_depth - r.getOrigin().z) / r.getDirection().z;
    }
    else if (_rectType == XZ)
    {
        t = (_depth - r.getOrigin().y) / r.getDirection().y;
    }
    else
    {
        t = (_depth - r.getOrigin().x) / r.getDirection().x;
    }

    if (t < tMin || t > tMax)
    {
        return false;
    }

    // I don't think I can get around the if statements here :(
    if (_rectType == XY)
    {
        dim1 = r.getOrigin().x + t * r.getDirection().x;
        dim2 = r.getOrigin().y + t * r.getDirection().y;
    }
    else if (_rectType == XZ)
    {
        dim1 = r.getOrigin().x + t * r.getDirection().x;
        dim2 = r.getOrigin().z + t * r.getDirection().z;
    }
    else
    {
        dim1 = r.getOrigin().y + t * r.getDirection().y;
        dim2 = r.getOrigin().z + t * r.getDirection().z;
    }


    if (dim1 < _dim1_0 || dim1 > _dim1_1 || dim2 < _dim2_0 || dim2 > _dim2_1)
    {
        return false;
    }

    hitRecord.u = (dim1 - _dim1_0) / (_dim1_1 - _dim1_0);
    hitRecord.v = (dim2 - _dim2_0) / (_dim2_1 - _dim2_0);
    hitRecord.normal = _normal;
    hitRecord.t = t;
    hitRecord.pMaterial = _pMaterial;
    hitRecord.p = r.pointAtParameter(t);

    return true;
}

std::string Rect::toString()
{
    std::string s = "[RECT] ";

    s += "Length: " + std::to_string((int)_length) + " "
        + "Width: " + std::to_string((int)_length) + " "
        + "Position: " + std::to_string(_position.x)
                         + " " + std::to_string(_position.y)
                           + " " + std::to_string(_position.z);

    return s;
}