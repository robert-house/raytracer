#pragma once

#include <chrono>
#include "Vec3.h"

class Random
{
public:
    Random();
    ~Random();
    double getRandom();
    Vec3 randomInUnitSphere();
    Vec3 randomInUnitDisk();

    // TODO: Feature to pre-generate a blue noise mask to use as the
    //       sampling pattern per pixel

private:
    uint32_t rngState = 1024;
};