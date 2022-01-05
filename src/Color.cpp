#include "Color.h"


Color::Color()
{
    // Default color is black
    colorVector = glm::vec3(0.0f, 0.0f, 0.0f);
}

Color::Color(double red, double green, double blue)
{
    colorVector = glm::vec3(red, green, blue);
}

Color::~Color() {}

double Color::GetR()
{
    return colorVector.x;
}

double Color::GetG()
{
    return colorVector.y;
}

double Color::GetB()
{
    return colorVector.z;
}