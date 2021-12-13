#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "Random.h"
#include <math.h>
#include <random>

constexpr double PI = 3.14159265358979323846;

class Camera
{
public:
    Camera();
    Camera(Vec3 cameraAt, Vec3 lookAt, Vec3 vUp, double vfov, double aspect, double aperture, double focus_dist);
    ~Camera();

    void Init();
    void UpdateVFOV(double fov);
    void UpdateCameraPosition(double x, double y, double z);
    void UpdateCameraDirection(double x, double y, double z);
    void updateAspectRatio(double aspectRatio);

    Ray getRay(double u, double v);

    Vec3 _origin;
    Vec3 _lookDirection;
    Vec3 _upVector;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
private:
    double _aperture;
    double _fieldOfView;
    double _focusDistance;
    double _aspectRatio;
    Vec3 u, v, w;
    Vec3 randInUnitDisk();
    double GetRandom();
    //Random random;
};

/*
#pragma once
#include "Vec3.h"
#include "Ray.h"
#include <math.h>
#include "gdt/math/vec.h"

constexpr double PI = M_PI;

class Camera
{
public:
    Camera();
    Camera(Vec3 cameraAt, Vec3 lookAt, Vec3 vUp, double vfov, double aspect, double aperture, double focus_dist);
    Camera(gdt::vec3f from, gdt::vec3f at, gdt::vec3f up);
    ~Camera();

    void Init();
    void UpdateVFOV(double fov);
    void UpdateCameraPosition(double x, double y, double z);
    void UpdateCameraDirection(double x, double y, double z);

    Ray getRay(double u, double v);

    Vec3 _origin;
    Vec3 _lookDirection;
    Vec3 _upVector;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;

    gdt::vec3f from;
    gdt::vec3f at;
    gdt::vec3f up;

private:
    double _aperture;
    double _fieldOfView;
    double _focusDistance;
    double _aspectRatio;
    Vec3 u, v, w;
    Vec3 randInUnitDisk();
    double GetRandom();
};
*/