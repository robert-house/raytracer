#include "Lambertian.h"

Lambertian::Lambertian(double albedoR, double albedoG, double albedoB)
{
    _albedo = Vec3(albedoR, albedoG, albedoB);
}

bool Lambertian::scatterRay(const Ray& rayIn, const HitRecord& hitRecord, Vec3& attenuation, Ray& scattered)
{
    Vec3 target = hitRecord.p + hitRecord.normal + random.randomInUnitSphere();
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    attenuation = _albedo;
    return true;
}

Vec3 Lambertian::getAlbedo()
{
    return _albedo;
}

// FIXME: 36% runtime
// TODO: Move cool function to random
Vec3 Lambertian::randomDirection()
{
    //Vec3 p;
    // (x^2 + y^2 + z^2) < 1


    // What the hell was I thinking?
    // eg. vec3 rand = 1 1 1
    //      2.0 * 1 1 1 = 2 2 2 - 1 1 1 = 1 1 1
    /*
    do
    {
        //p = 2.0 * Vec3(getRandom(), getRandom(), getRandom()) - Vec3(1, 1, 1);
    }
    while (p.squared_length() >= 1.0);

    return p; */

    // COOL LIGHT TRANSPORT ALGO
    /*
    double rand = random.getRandom(0.0, 1.0);

    double theta = 2 * M_PI * rand;
    double phi = acos(2.0 * rand - 1);
    double r = cbrt(rand);
    double x = r * sin(phi) * cos(theta);
    double y = r * sin(phi) * sin(theta);
    double z = r * cos(phi);

    return Vec3(x, y, z);
    */

    // Current optimized Algorithm
    double theta = 2 * M_PI * random.getRandom(0.0, 1.0);
    double phi = acos(2.0 * random.getRandom(0.0, 1.0) - 1);
    double r = cbrt(random.getRandom(0.0, 1.0));
    double x = r * sin(phi) * cos(theta);
    double y = r * sin(phi) * sin(theta);
    double z = r * cos(phi);

    return Vec3(x, y, z);

    /*
    double u = random.getRandom(0.0, 1.0);
    double v = random.getRandom(0.0, 1.0);
    double theta = 2.0 * M_PI * u;
    double phi = acos(2.0 * v - 1);
    double r = cbrt(random.getRandom(0.0, 1.0));
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);
    double sinPhi = sin(phi);
    double cosPhi = cos(phi);

    return Vec3(r * sinPhi * cosTheta, r * sinPhi * sinTheta, r * cosPhi); */
}