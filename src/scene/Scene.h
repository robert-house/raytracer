//=========================================================================//
// Scene.h                                                                 //
//=========================================================================//
// Representation of a renderable scene                                    //
// Robert House                                                            //
// 16 FEB 2020                                                             //
//-------------------------------------------------------------------------//
// Scene: Includes functionality to both load from file and to reset the   //
//        current state                                                    //
//-------------------------------------------------------------------------//

#pragma once
#include <vector>
#include "../shapes/Shape.h"
#include "../shapes/Sphere.h"
#include "../HitRecord.h"
#include "../shapes/ShapeFactory.h"

// REMOVE LATER
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Scene
{
public:
    Scene(Log* log);
    ~Scene();

    void AddToScene(Shape* shape);
    bool TestForCollision(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord);
    int getSize() { return _entities.size(); }
    bool LoadFromFile(std::string);
    void GenerateRandomScene();
    void resetScene(std::string filepath);

private:
    std::vector<Shape*> _entities;
    ShapeFactory _shapeFactory;
    Log* _pLog;
};