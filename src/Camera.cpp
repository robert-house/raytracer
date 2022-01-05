#include "Camera.h"

Camera::Camera()
{
    _aperture = 0;
}

Camera::Camera(glm::vec3 cameraAt, glm::vec3 lookAt, glm::vec3 vUp, float vfov, float aspect, float aperture, float focusDistance)
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
    w = glm::normalize(_origin - _lookDirection);
    u = glm::normalize(glm::cross(_upVector, w));
    v = glm::cross(w, u);

    float half_height = tan(_fieldOfView / 2);
    float half_width = _aspectRatio * half_height;

    lowerLeftCorner = _origin - half_width * _focusDistance * u - half_height * _focusDistance * v - _focusDistance * w;
    horizontal = 2 * half_width * _focusDistance * u;
    vertical = 2 * half_height * _focusDistance * v;
}

void Camera::UpdateVFOV(float fov)
{
    _fieldOfView = fov * PI / 180;
}

void Camera::UpdateCameraPosition(float x, float y, float z)
{
    _origin.x = x;
    _origin.y = y;
    _origin.z = z;

    Init();
}

void Camera::UpdateCameraDirection(float x, float y, float z)
{
    _lookDirection.x = x;
    _lookDirection.y = y;
    _lookDirection.z = z;

    Init();
}

void Camera::updateAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
    Init(); // Reinit
}

Ray Camera::getRay(float s, float t)
{
    glm::vec3 rd = _aperture / 2 * randInUnitDisk(); // TODO: Fix bug regarding crash with Random object
    glm::vec3 offset = u * rd.x + v * rd.y;
    return Ray(_origin + offset, lowerLeftCorner + s * horizontal + t * vertical - _origin - offset);
}

// FIXME: 9% runtime
glm::vec3 Camera::randInUnitDisk()
{
    /*glm::vec3 p;
    do
    {
        p = 2.0 * glm::vec3(GetRandom(), GetRandom(), 0) - glm::vec3(1, 1, 0);
    } while (glm::dot(p, p) >= 1.0);

    return p; */

    // TODO: Get from 4% to lower
    float r = sqrtf(GetRandom());
    float theta = 2.0f * M_PI * GetRandom();
    float x = r * cos(theta);
    float y = r * sin(theta);

    return glm::vec3(x, y, 0);
}

float Camera::GetRandom()
{
    //thread_local std::mt19937 generator(std::random_device{}());
    //std::uniform_real_distribution<float> distribution(0.0, 1.0);
    //return distribution(generator);

    return 1.0f; // TODO: Fix Random Issue
}

/*
#include "Camera.h"

Camera::Camera()
{
    _aperture = 0;
}

Camera::Camera(glm::vec3 cameraAt, glm::vec3 lookAt, glm::vec3 vUp, float vfov, float aspect, float aperture, float focusDistance)
{
    _aperture      = aperture;
    _origin        = cameraAt;
    _lookDirection = lookAt;
    _upVector      = vUp;
    _focusDistance = focusDistance;
    _aspectRatio   = aspect;

    // math lib stuff
    from = gdt::glm::vec3f(cameraAt.x(), cameraAt.y(), cameraAt.z());
    at   = gdt::glm::vec3f(lookAt.x(), lookAt.y(), lookAt.z());
    up   = gdt::glm::vec3f(vUp.x(), vUp.y(), vUp.z());

    // Setup FOV
    UpdateVFOV(vfov);
    Init();
}

Camera::Camera(gdt::glm::vec3f infrom, gdt::glm::vec3f inat, gdt::glm::vec3f inup)
{
    // math lib stuff
    from = infrom;
    at = inat;
    up = inup;
}

Camera::~Camera() {}

void Camera::Init()
{
    w = glm::vec3::unit_vector(_origin - _lookDirection);
    u = glm::vec3::unit_vector(glm::vec3::cross(_upVector, w));
    v = glm::vec3::cross(w, u);

    float half_height = tan(_fieldOfView / 2);
    float half_width = _aspectRatio * half_height;

    lowerLeftCorner = _origin - half_width * _focusDistance * u - half_height * _focusDistance * v - _focusDistance * w;
    horizontal = 2 * half_width * _focusDistance * u;
    vertical = 2 * half_height * _focusDistance * v;
}

void Camera::UpdateVFOV(float fov)
{
    _fieldOfView = fov * PI / 180;
}

void Camera::UpdateCameraPosition(float x, float y, float z)
{
    _origin.e[0] = x;
    _origin.e[1] = y;
    _origin.e[2] = z;

    Init();
}

void Camera::UpdateCameraDirection(float x, float y, float z)
{
    _lookDirection.e[0] = x;
    _lookDirection.e[1] = y;
    _lookDirection.e[2] = z;

    Init();
}

Ray Camera::getRay(float s, float t)
{
    glm::vec3 rd = _aperture / 2 * randInUnitDisk();
    glm::vec3 offset = u * rd.x() + v * rd.y();
    return Ray(_origin + offset, lowerLeftCorner + s * horizontal + t * vertical - _origin - offset);
}

glm::vec3 Camera::randInUnitDisk()
{
    glm::vec3 p;
    do
    {
        p = 2.0 * glm::vec3(GetRandom(), GetRandom(), 0) - glm::vec3(1, 1, 0);
    }
    while (glm::vec3::dot(p, p) >= 1.0);

    return p;
}

float Camera::GetRandom()
{
    return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}
*/