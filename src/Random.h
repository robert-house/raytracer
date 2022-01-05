#pragma once

#include <chrono>
#include <math.h>
#include <glm/vec3.hpp>

class Random
{
public:
    Random();
    ~Random();
    double getRandom();
    glm::vec3 randomInUnitSphere();
    glm::vec3 randomInUnitDisk();

    // TODO: Feature to pre-generate a blue noise mask to use as the
    //       sampling pattern per pixel

private:
    uint32_t rngState = 1024;
};