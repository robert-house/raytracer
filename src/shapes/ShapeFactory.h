#pragma once
#include <deque>
#include <string>
#include <random>
#include "Shape.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Rect.h"
#include "../materials/MaterialFactory.h"
#include "../materials/Material.h"
#include "..//Vec3.h"

#include "../logging/Log.h"

class ShapeFactory
{
public:
    ShapeFactory();
    ShapeFactory(Log* log);
    ~ShapeFactory();

    Shape* MakeShape(std::deque<std::string>& shapeDescription);
    Shape* GetRandomShape(int seedA, int seedB);

private:
    MaterialFactory matFac;
    Log* _pLog;
    double GetRandom();
};