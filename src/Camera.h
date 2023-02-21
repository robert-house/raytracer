#pragma once
#include "Ray.h"
#include "Random.h"
#include <math.h>
#include <random>
#include <glm/glm.hpp>

constexpr float PI = 3.14159265358979323846;

class Camera
{
public:
    Camera();
    Camera(glm::vec3 cameraAt, glm::vec3 lookAt, glm::vec3 vUp, float vfov, float aspect, float aperture, float focus_dist);
    ~Camera();

    void Init();
    void UpdateVFOV(float fov);
    void UpdateCameraPosition(float x, float y, float z);
    void UpdateCameraDirection(float x, float y, float z);
    void updateAspectRatio(float aspectRatio);

    Ray getRay(float u, float v);

    glm::vec3 _origin;
    glm::vec3 _lookDirection;
    glm::vec3 _upVector;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
private:
    float _aperture;
    float _fieldOfView;
    float _focusDistance;
    float _aspectRatio;
    glm::vec3 u, v, w;
    glm::vec3 randInUnitDisk();
    float GetRandom();
    uint32_t rngState = 2048;
    //Random random;
};

/*
#pragma once
#include "glm::vec3.h"
#include "Ray.h"
#include <math.h>
#include "gdt/math/vec.h"

constexpr float PI = M_PI;

class Camera
{
public:
    Camera();
    Camera(glm::vec3 cameraAt, glm::vec3 lookAt, glm::vec3 vUp, float vfov, float aspect, float aperture, float focus_dist);
    Camera(gdt::glm::vec3f from, gdt::glm::vec3f at, gdt::glm::vec3f up);
    ~Camera();

    void Init();
    void UpdateVFOV(float fov);
    void UpdateCameraPosition(float x, float y, float z);
    void UpdateCameraDirection(float x, float y, float z);

    Ray getRay(float u, float v);

    glm::vec3 _origin;
    glm::vec3 _lookDirection;
    glm::vec3 _upVector;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;

    gdt::glm::vec3f from;
    gdt::glm::vec3f at;
    gdt::glm::vec3f up;

private:
    float _aperture;
    float _fieldOfView;
    float _focusDistance;
    float _aspectRatio;
    glm::vec3 u, v, w;
    glm::vec3 randInUnitDisk();
    float GetRandom();
};
*/