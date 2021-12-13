#pragma once
#include "Vec3.h"

class Color
{
public:
    Color();
    Color(double red, double green, double blue);
    ~Color();

    double GetR();
    double GetG();
    double GetB();
private:
    Vec3 colorVector;
};