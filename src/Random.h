#pragma once

#include <random>
#include <chrono>
#include "Vec3.h"

class Random
{
public:
    Random();
    ~Random();
    double getRandom();
    double getRandom(double from, double to);
    Vec3 randomInUnitSphere();
    Vec3 randomInUnitDisk();

    // TODO: Feature to pre-generate a blue noise mask to use as the
    //       sampling pattern per pixel

private:
    std::mt19937 rnd;
    std::uniform_real_distribution<double> distribution;
    std::uniform_real_distribution<double> customDistribution;
};