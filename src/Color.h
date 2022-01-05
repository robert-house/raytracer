#pragma once
#include <glm/vec3.hpp>

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
    glm::vec3 colorVector;
};