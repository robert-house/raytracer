#include "Material.h"

float Material::getRandom()
{
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
    //return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
}