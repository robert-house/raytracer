//=========================================================================//
// RayTracer.h                                                             //
//=========================================================================//
// Simple raytracer                                                        //
// Robert House                                                            //
// 23 NOV 2019                                                             //
//-------------------------------------------------------------------------//
// RayTracer: Module to encapsulate raytraced rendering                    //
//-------------------------------------------------------------------------//

#pragma once
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <float.h>

#include "Renderer.h"
#include "RenderTarget.h"
#include "../scene/Scene.h"
#include "../Camera.h"
#include "../HitRecord.h"
#include "..//Ray.h"
#include "..//Camera.h"
#include "..//Random.h"
#include <glm/glm.hpp>
//#include "../config/CVarDescriptor.h"

struct RTProfiling
{
    unsigned long numRays = 0;
    long thread = -1;
};

class RayTracer final : public Renderer
{
public:
    RayTracer();
    RayTracer(std::shared_ptr<Log> log);
    RayTracer(const RayTracer& rayTracerCopy); // Copy Constructor
    ~RayTracer();

    void init() override;
    void update() override;
    void render() override;
    void destroy() override;
    void reInit() override;

    void changeResolution(int width, int height) override;
    void registerScene(Scene* pScene);
    void registerCamera(Camera* pCamera);

private:
    // CVARs
    CVar* r_samplesPerPixel;    // Default 10
    CVar* r_numberOfBounces;    // Default 50
    CVar* c_numberOfThreads;    // Default 1
    CVar* r_flatShading;        // Default off
    CVar* r_renderingMode;
    CVar* r_disableShading;
    CVar* r_tracingMode;

    // External Pointers. DONT DELETE!
    Camera*       _pCamera;
    Scene*        _pScene;

    // External Objects
    RenderTarget  _renderTarget;

    // Profiling Variables
    unsigned long _numRays;
    unsigned long _bouncedRays;
    unsigned long _raysEmitted;

    // Mutices
    std::mutex    _mut_rt;

    // Private members
    glm::vec3 shootRay(const Ray& r, Scene* scene, int depth);
    glm::vec3 iterativeShootRay(Ray& ray, Scene* scene);
    glm::vec3 randomPointInSphere();
    void renderScene(int samples);
    //void mtRender(int samples);
    double getRandom();
    void loadCVars();


    // Experimental
    long renderSLI();
    long renderSplitSamples();
    void drawScanline(int samples, int scanLine, std::vector<glm::vec3>& line);
    std::string renderingMode();
    Random random;
};
