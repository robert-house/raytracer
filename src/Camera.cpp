#include "Camera.h"

Camera::Camera()
{
    _aperture = 0;
}

Camera::Camera(Vec3 cameraAt, Vec3 lookAt, Vec3 vUp, double vfov, double aspect, double aperture, double focusDistance)
{
    _aperture = aperture;
    _origin = cameraAt;
    _lookDirection = lookAt;
    _upVector = vUp;
    _focusDistance = focusDistance;
    _aspectRatio = aspect;

    // Setup FOV
    UpdateVFOV(vfov);
    Init();
}

Camera::~Camera() {}

// TODO: Create a reinit function
void Camera::Init()
{
    w = Vec3::unit_vector(_origin - _lookDirection);
    u = Vec3::unit_vector(Vec3::cross(_upVector, w));
    v = Vec3::cross(w, u);

    double half_height = tan(_fieldOfView / 2);
    double half_width = _aspectRatio * half_height;

    lowerLeftCorner = _origin - half_width * _focusDistance * u - half_height * _focusDistance * v - _focusDistance * w;
    horizontal = 2 * half_width * _focusDistance * u;
    vertical = 2 * half_height * _focusDistance * v;
}

void Camera::UpdateVFOV(double fov)
{
    _fieldOfView = fov * PI / 180;
}

void Camera::UpdateCameraPosition(double x, double y, double z)
{
    _origin.e[0] = x;
    _origin.e[1] = y;
    _origin.e[2] = z;

    Init();
}

void Camera::UpdateCameraDirection(double x, double y, double z)
{
    _lookDirection.e[0] = x;
    _lookDirection.e[1] = y;
    _lookDirection.e[2] = z;

    Init();
}

void Camera::updateAspectRatio(double aspectRatio)
{
    _aspectRatio = aspectRatio;
    Init(); // Reinit
}

Ray Camera::getRay(double s, double t)
{
    Vec3 rd = _aperture / 2 * randInUnitDisk(); // TODO: Fix bug regarding crash with Random object
    Vec3 offset = u * rd.x() + v * rd.y();
    return Ray(_origin + offset, lowerLeftCorner + s * horizontal + t * vertical - _origin - offset);
}

// FIXME: 9% runtime
Vec3 Camera::randInUnitDisk()
{
    /*Vec3 p;
    do
    {
        p = 2.0 * Vec3(GetRandom(), GetRandom(), 0) - Vec3(1, 1, 0);
    } while (Vec3::dot(p, p) >= 1.0);

    return p; */

    // TODO: Get from 4% to lower
    float r = sqrtf(GetRandom());
    float theta = 2 * M_PI * GetRandom();
    float x = r * cos(theta);
    float y = r * sin(theta);

    return Vec3(x, y, 0);
}

double Camera::GetRandom()
{
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

/*
#include "Camera.h"

Camera::Camera()
{
    _aperture = 0;
}

Camera::Camera(Vec3 cameraAt, Vec3 lookAt, Vec3 vUp, double vfov, double aspect, double aperture, double focusDistance)
{
    _aperture      = aperture;
    _origin        = cameraAt;
    _lookDirection = lookAt;
    _upVector      = vUp;
    _focusDistance = focusDistance;
    _aspectRatio   = aspect;

    // math lib stuff
    from = gdt::vec3f(cameraAt.x(), cameraAt.y(), cameraAt.z());
    at   = gdt::vec3f(lookAt.x(), lookAt.y(), lookAt.z());
    up   = gdt::vec3f(vUp.x(), vUp.y(), vUp.z());

    // Setup FOV
    UpdateVFOV(vfov);
    Init();
}

Camera::Camera(gdt::vec3f infrom, gdt::vec3f inat, gdt::vec3f inup)
{
    // math lib stuff
    from = infrom;
    at = inat;
    up = inup;
}

Camera::~Camera() {}

void Camera::Init()
{
    w = Vec3::unit_vector(_origin - _lookDirection);
    u = Vec3::unit_vector(Vec3::cross(_upVector, w));
    v = Vec3::cross(w, u);

    double half_height = tan(_fieldOfView / 2);
    double half_width = _aspectRatio * half_height;

    lowerLeftCorner = _origin - half_width * _focusDistance * u - half_height * _focusDistance * v - _focusDistance * w;
    horizontal = 2 * half_width * _focusDistance * u;
    vertical = 2 * half_height * _focusDistance * v;
}

void Camera::UpdateVFOV(double fov)
{
    _fieldOfView = fov * PI / 180;
}

void Camera::UpdateCameraPosition(double x, double y, double z)
{
    _origin.e[0] = x;
    _origin.e[1] = y;
    _origin.e[2] = z;

    Init();
}

void Camera::UpdateCameraDirection(double x, double y, double z)
{
    _lookDirection.e[0] = x;
    _lookDirection.e[1] = y;
    _lookDirection.e[2] = z;

    Init();
}

Ray Camera::getRay(double s, double t)
{
    Vec3 rd = _aperture / 2 * randInUnitDisk();
    Vec3 offset = u * rd.x() + v * rd.y();
    return Ray(_origin + offset, lowerLeftCorner + s * horizontal + t * vertical - _origin - offset);
}

Vec3 Camera::randInUnitDisk()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(GetRandom(), GetRandom(), 0) - Vec3(1, 1, 0);
    }
    while (Vec3::dot(p, p) >= 1.0);

    return p;
}

double Camera::GetRandom()
{
    return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
}
*/