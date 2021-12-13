#pragma once
#include <deque>
#include <string>
#include <random>
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "DiffuseEmitter.h"
#include "MissingMat.h"

#include "../logging/Log.h"

class MaterialFactory
{
public:
    MaterialFactory();
    MaterialFactory(Log* log);
    ~MaterialFactory();

    Material* MakeMaterial(std::deque<std::string>& materialDescription);
    Material* GetRandomMaterial();
private:
    double GetRandom();
    Log* _pLog;
};