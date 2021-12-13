#include "Color.h"


Color::Color()
{
    // Default color is black
    colorVector = Vec3(0.0, 0.0, 0.0);
}

Color::Color(double red, double green, double blue)
{
    colorVector = Vec3(red, green, blue);
}

Color::~Color() {}

double Color::GetR()
{
    return colorVector.x();
}

double Color::GetG()
{
    return colorVector.y();
}

double Color::GetB()
{
    return colorVector.z();
}