#include "Random.h"

Random::Random()
{
}

Random::~Random() {} // Nothing to break down, leave empty

// New and FAST!
// https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
double Random::getRandom()
{
    uint32_t state = rngState;
    rngState = rngState * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (double)((word >> 22u) ^ word) / UINT32_MAX;
}

Vec3 Random::randomInUnitSphere()
{
    double theta = 2 * M_PI * getRandom();
    double phi = acos(2.0 * getRandom() - 1);
    double r = cbrt(getRandom());
    double x = r * sin(phi) * cos(theta);
    double y = r * sin(phi) * sin(theta);
    double z = r * cos(phi);

    return Vec3(x, y, z);
}

Vec3 Random::randomInUnitDisk()
{
    float r = sqrtf(getRandom());
    float theta = 2 * M_PI * getRandom();
    float x = r * cos(theta);
    float y = r * sin(theta);

    return Vec3(x, y, 0);
}