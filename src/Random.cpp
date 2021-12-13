#include "Random.h"

Random::Random() : rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
    distribution = std::uniform_real_distribution<double>(0.0, 1.0);
    //customDistribution = std::uniform_real_distribution<double>(from, to);
    //customDistribution = std::uniform_real_distribution<double>(0.0, 1.0);
}

Random::~Random() {}

double Random::getRandom()
{
    return distribution(rnd);
}

double Random::getRandom(double from, double to)
{
    //customDistribution.reset();
    //customDistribution = std::uniform_real_distribution<double>(from, to);
    //return customDistribution(rnd);
    return distribution(rnd);
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