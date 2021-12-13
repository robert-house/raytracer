#include "RayTracer.h"

RayTracer::RayTracer()
    : Renderer(0,0,L"Why?", std::shared_ptr<Log>(new ConsoleLog()))
{
    // Default Constructor
}

// TODO: Revisit the super constructor. This doesn't make any sense if we are going to manually configure it
RayTracer::RayTracer(std::shared_ptr<Log> log) : Renderer(0, 0, L"", log)
{
    init();
}

// Copy constructor
RayTracer::RayTracer(const RayTracer& rayTracerCopy) : Renderer(0, 0, L"Why?", _pLog)
{
}

// TODO: Implement destructor
RayTracer::~RayTracer()
{
}

void RayTracer::init()
{
    // Init CVars for this class
    loadCVars();

    // RenderTarget Setup
    _renderTarget = RenderTarget(r_width->getValue(), r_height->getValue());

    // Init profiling stuff
    _numRays     = 0;
    _bouncedRays = 0;
    _raysEmitted = 0;

    // Init pointers to nullptr
    _pCamera = nullptr;
    _pScene = nullptr;

    _pLog->log(Log::INFO, "Init completed successfully!");
}

void RayTracer::update()
{
    // TODO: Pass ray from camera into here to decouple the class
    //
    std::cout << "CALLED FROM POINTER!" << std::endl;
}

void RayTracer::render()
{
    // Pre-condition checks
    if (_pCamera == nullptr)
    {
        _pLog->error("Camera was not set. Rendering Will NOT Continue");
        return;
    }

    if (_pScene == nullptr)
    {
        _pLog->error("Scene was not set. Rendering Will NOT Continue");
        return;
    }

    // If any of the resolution values were changed, re-init camera and rendertarget
    if(r_height->wasModified() || r_width->wasModified())
    {
        _renderTarget.resizeTarget((unsigned int)r_width->getValue(), (unsigned int)r_height->getValue());
        _pCamera->updateAspectRatio(r_width->getValue() / r_height->getValue());
    }

    long elapsedTime = 0;

    // Log Rendering Info
    _pLog->info("================ [Rendering] ================");
    _pLog->info("Resolution       : " + std::to_string((int)r_width->getValue()) + "x"
                    + std::to_string((int)r_height->getValue()));
    _pLog->info("Samples per Pixel: " + std::to_string((int)r_samplesPerPixel->getValue()));
    _pLog->info("Number of Bounces: " + std::to_string((int)r_numberOfBounces->getValue()));
    _pLog->info("Rendering Mode   : " + renderingMode());
    _pLog->info("Number of Threads: " + std::to_string((int)c_numberOfThreads->getValue()));
    _pLog->info("=============================================");

    if (r_renderingMode->getValue() == 1)
    {
        elapsedTime = renderSplitSamples();
    }
    else
    {
        elapsedTime = renderSLI();
    }


    // Print out number of rays fired
    std::cout << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "                  METRICS                    " << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << " Number of Threads    : " << c_numberOfThreads->getValue() << std::endl;
    std::cout << " Total Elapsed Time   : " << elapsedTime << " [ms]"<< std::endl;
    //std::cout << " Total Rays           : " << _numRays << std::endl;
    //std::cout << " Rays per Millisecond : " << double(_numRays) / double(elapsedTime) << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << std::endl;

    // TODO: When SDL2 is hooked up, output to window rather than disk
    _renderTarget.writeFrameToDisk("frame");
}

void RayTracer::destroy() {}

void RayTracer::reInit() {}

void RayTracer::changeResolution(int width, int height) {}

void RayTracer::registerScene(Scene* pScene)
{
    _pScene = pScene;
}

void RayTracer::registerCamera(Camera* pCamera)
{
    _pCamera = pCamera;
}

// Private Members
Vec3 RayTracer::shootRay(const Ray& r, Scene* scene, int depth)
{
    HitRecord hitRecord;
    Vec3 pixelColor;

    // Do hit detection on Shapes
    // TODO: Max Float or Double
    // TODO: Iterative Renderer
    if (scene->TestForCollision(r, 0.001, 1000000000000000000.0, hitRecord))
    {
        Ray scattered;
        Vec3 attenuation;
        Vec3 emitted = hitRecord.pMaterial->emittRay(hitRecord.u, hitRecord.v, hitRecord.p);

        if (r_disableShading->getValue() == 1)
        {
            return hitRecord.pMaterial->getAlbedo();
        }

        // Do recursive call for secondary bounces
        if (depth < r_numberOfBounces->getValue() && hitRecord.pMaterial->scatterRay(r, hitRecord, attenuation, scattered))
        {
            // For a non-emissive material, emitted will always be zero and scatter_ray will return false
            // due to how this currently works
            if (r_flatShading->getValue() > 0)
            {
                return emitted + attenuation;
            }
            else
            {
                return emitted + attenuation * shootRay(scattered, scene, depth + 1);
                //return emitted + attenuation * emitted + attenuation * 0.0f;
            }
        }
        else
        {
            // In this current system, we can assume that this is called when we emit light
            return emitted;
        }
    }
    else
    {
        //Vec3 unitDirection = Vec3::unit_vector(r.getDirection());
        //double t = 0.5 * (unitDirection.y() + 1.0);

        // Lerp Algorithm: blendedValue = (1 - t) * start + t * end
        //pixelColor = (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);

        // NO GLOBAL LIGHTSOURCE
        return Vec3(0, 0, 0);
    }

    // Do internal color space correction here
    return pixelColor;
}

Vec3 RayTracer::iterativeShootRay(Ray& ray, Scene* scene)
{
    HitRecord hitRecord;
    Vec3 color;
    Vec3 attenuation;
    Vec3 emitted;
    Vec3 totalAttenuation(1,1,1);
    int depth = 0;
    int maxBounces = r_numberOfBounces->getValue();

    // TODO: Get rid of magic number
    while (depth < maxBounces && scene->TestForCollision(ray, 0.01, DBL_MAX, hitRecord))
    {
        // Grab the emitted term from this material
        emitted = hitRecord.pMaterial->emittRay(hitRecord.u, hitRecord.v, hitRecord.p);

        // Calculate the color using the current emmited term and the previous
        // multiplicative attenuation term
        color += emitted * totalAttenuation;

        // Calculate new multiplicative attenuation term only if the ray scatters. If not,
        // return the color that we have already computed
        if (hitRecord.pMaterial->scatterRay(ray, hitRecord, attenuation, ray))
        {
            totalAttenuation *= attenuation;
        }
        else
        {
            return color;
        }

        // increment our depth
        depth++;
    }

    // Return our complete color
    return color;
}

void RayTracer::renderScene(int samples)
{
    // Loop to sweep the "beam" from lower left to upper right
    // Important to view it as height x width so we can not thrash memory. This is because
    // we are using a 2d array and the memory is contiguous on ROWS
    //          i
    // o -------------->
    //   |
    //   |
    // j |
    //   |
    //   V

    // This took a bit of figuring out, but the errors I was getting with >1 threads
    // were due to rand() not being thread safe. Each thread was calling rand() and
    // getting either the same number, numbers in sequence, or numbers too similar
    // and the output was looking as if we had samplesPerPixel / numTreads samples
    // TODO: Explore better random number generators

    // This ended up being the fastest solution. See commented out sections
    // for "proper" RN generation. I was probably using it wrong, but it was
    // still cutting performance in half
    //srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    double u = 0;
    double v = 0;
    int height = r_height->getValue();
    int width = r_width->getValue();
    Ray primaryRay;
    Vec3 pixelColor = Vec3();

    // TODO: Tile renderer
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            pixelColor.e[0] = 0;
            pixelColor.e[1] = 0;
            pixelColor.e[2] = 0;

            if (samples < 2)
            {
                u = double(i / width);
                v = double(j / height);
                // Transform point in screenspace to world space
                // TODO: Remove this statement from here
                // TODO: Check if calling getRay is causing performance issues
                primaryRay = _pCamera->getRay(u, v);

                // Accumulate Pixel Color
                pixelColor += shootRay(primaryRay, _pScene, 0);
            }
            else
            {
                // Take multiple samples per pixel
                for (int s = 0; s < samples; s++)
                {
                    // Compute both the U and V vectors for this point in screenspace
                    u = double(i + getRandom()) / width;
                    v = double(j + getRandom()) / height;
                    // Transform point in screenspace to world space
                    // TODO: Remove this statement from here
                    primaryRay = _pCamera->getRay(u, v);

                    // Accumulate Pixel Color
                    pixelColor += shootRay(primaryRay, _pScene, 0);
                }
            }

            // Write to RenderTarget
            // This may not be the best way to do this. It might make sense to have n number of render
            // targets for n threads or at least have some sort of buffer system so we can combine
            // the accumulated output before we tonemap or modify the colorspace
            // TODO: Investigate
            _renderTarget.accumulate(j, i, Vec3(pixelColor.x(), pixelColor.y(), pixelColor.z()));
        }
    }
}

// TO DO WORK IN PROGRESS
Vec3 RayTracer::randomPointInSphere()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(getRandom(), getRandom(), getRandom()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}

double RayTracer::getRandom()
{
    //return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(-0.5, 0.5); // TODO: Check to see if there is a difference
    return distribution(generator);
}

double doubleRand(double min, double max)
{
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

void RayTracer::loadCVars()
{
    std::string description = "";
    // Start assignment
    // Resolution Setup is in the base class
    description = "Change the number of samples taken per pixel e.g. 32 64 128 256";
    r_samplesPerPixel = Config::getInstance().registerCVar("r_samplesPerPixel", 256.0, true, description);

    description = "Change the number of bounces allowed per ray e.g. 2 4 8 16";
    r_numberOfBounces = Config::getInstance().registerCVar("r_numberOfBounces", 2.0, true, description);

    description = "Enable flat shading for debugging purposes";
    r_flatShading = Config::getInstance().registerCVar("r_flatShading", 0.0, false, description);

    description = "Change the number of threads to use while rendering e.g. 2 4 8 16";
    c_numberOfThreads = Config::getInstance().registerCVar("c_numberOfThreads", 8.0, true, description); // TODO: Move this elsewhere!

    description = "Choose Rendering Mode 1) Normal 2) SLI";
    r_renderingMode = Config::getInstance().registerCVar("r_renderingMode", 1, true, description);

    description = "Disable light transport shading";
    r_disableShading = Config::getInstance().registerCVar("r_disableShading", 0, true, description);

    CVarDescriptor descriptor;
    descriptor.name = "r_tracingMode";
    descriptor.archivable = true;
    descriptor.defaultValue = 0.0;
    descriptor.description = "Toggle between recursive and iterative raytracing";

    r_tracingMode = Config::getInstance().registerCVar(descriptor);
}

/// ================================
/// EXPERIMENTAL
///=================================
long RayTracer::renderSLI()
{
    long elapsedTime = 0;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    unsigned int samples = r_samplesPerPixel->getValue();
    int height = r_height->getValue();
    int width = r_width->getValue();
    int threadCount = c_numberOfThreads->getValue();
    int nextLine = 0;
    std::vector<std::thread> threads(threadCount);

    // Setup Local Storage
    std::vector<std::vector<Vec3>> localBuffer(height, std::vector<Vec3>(width));

    // Single Threaded
    if (threadCount == 1)
    {
        begin = std::chrono::steady_clock::now();
        for (int i = 0; i < height; i++)
        {
            drawScanline(samples,i,localBuffer[i]);
        }
        end = std::chrono::steady_clock::now();
    }
    else
    {
        //std::cout << "START" << std::endl;

        // Tick
        begin = std::chrono::steady_clock::now();

        // Spawn Initial Threads
        for (int i = 0; i < threadCount; i++)
        {
            threads.push_back(std::thread(&RayTracer::drawScanline,this,samples,nextLine,std::ref(localBuffer[nextLine])));
            nextLine++;
        }

        //std::cout << "Inital Threads Started" << std::endl;

        // While there is still work to do, loop
        while (nextLine < height)
        {
            for (auto& t : threads)
            {
                if (t.joinable())
                {
                    //std::cout << "Thread Joined: " << t.get_id() << std::endl;
                    t.join(); // Join Thread

                    // Check to see if we can schedule work
                    if (nextLine < height)
                    {
                        t = std::thread(&RayTracer::drawScanline,
                                        this,
                                        samples,
                                        nextLine,
                                        std::ref(localBuffer[nextLine])); // Spawn Next work
                        //std::cout << "New Job Spawned: " << t.get_id() << std::endl;
                        nextLine++;
                    }
                }
            }
        }

        //std::cout << "Job Done: " << nextLine << " Lines" << std::endl;

        // Join all threads
        for (auto& t : threads)
        {
            if (t.joinable())
            {
                t.join(); // Join Thread
            }
        }

        //std::cout << "Rendering Complete" << std::endl;

        // Tock
        end = std::chrono::steady_clock::now();
    }



    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // Tonemap and copy to rendertarget
    for (int j = r_height->getValue() - 1; j >= 0; j--)
    {
        for (int i = 0; i < r_width->getValue(); i++)
        {
            Vec3 finalColor = localBuffer[j][i];

            // Divide the color values by the number of samples per pixel
            finalColor /= double(samples);

            // Cast back into good color space
            finalColor = Vec3(sqrt(finalColor.x()), sqrt(finalColor.y()), sqrt(finalColor.z()));

            // Tone map values greater than 255 to 255. This will prevent clipping
            int r = std::max(0, std::min(int(255.99 * finalColor.x()), 255));
            int g = std::max(0, std::min(int(255.99 * finalColor.y()), 255));
            int b = std::max(0, std::min(int(255.99 * finalColor.z()), 255));

            // Write to RenderTarget
            _renderTarget.draw(j, i, Vec3(r, g, b));
        }
    }

    //std::cout << "ToneMapping Complete" << std::endl;

    return elapsedTime;
}

long RayTracer::renderSplitSamples()
{
    // For my multi-threaded approach, we will split the number of samples per pixel
    // across the availible threads. This should give more or less a straight 800%
    // speedup vs one thread
    long elapsedTime = 0;
    // Setup profiling
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    unsigned int samplesPerThread = r_samplesPerPixel->getValue() / c_numberOfThreads->getValue();

    // Single Threaded
    if (c_numberOfThreads->getValue() == 1)
    {
        begin = std::chrono::steady_clock::now();
        renderScene(samplesPerThread);
        end = std::chrono::steady_clock::now();
    }
    else
    {
        std::vector<std::thread> threads;

        // Tick
        begin = std::chrono::steady_clock::now();

        // Spawn Threads
        // TODO: I suspect pthreads are killing performance on *nix. Prove it
        for (int i = 0; i < c_numberOfThreads->getValue(); i++)
        {
            threads.push_back(std::thread(&RayTracer::renderScene,
                                        this,
                                        samplesPerThread
                                        ));
        }

        for (auto& t : threads)
        {
            if (t.joinable())
                t.join();
        }

        // Tock
        end = std::chrono::steady_clock::now();
    }

    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // Fix up image
    for (int j = r_height->getValue() - 1; j >= 0; j--)
    {
        for (int i = 0; i < r_width->getValue(); i++)
        {
            Vec3 finalColor = _renderTarget.get(j, i);

            // Divide the color values by the number of samples per pixel
            finalColor /= double(r_samplesPerPixel->getValue());

            // Cast back into good color space
            finalColor = Vec3(sqrt(finalColor.x()), sqrt(finalColor.y()), sqrt(finalColor.z()));

            // Tone map values greater than 255 to 255. This will prevent clipping
            int r = std::max(0, std::min(int(255.99 * finalColor.x()), 255));
            int g = std::max(0, std::min(int(255.99 * finalColor.y()), 255));
            int b = std::max(0, std::min(int(255.99 * finalColor.z()), 255));

            // Write to RenderTarget
            _renderTarget.draw(j, i, Vec3(r, g, b));
        }
    }

    return elapsedTime;
}

void RayTracer::drawScanline(int samples, int scanLine, std::vector<Vec3>& line)
{
    // Loop to sweep the "beam" from lower left to upper right
    // Important to view it as height x width so we can not thrash memory. This is because
    // we are using a 2d array and the memory is contiguous on ROWS
    //          i
    // o -------------->
    //   |
    //   |
    // j |
    //   |
    //   V

    // This took a bit of figuring out, but the errors I was getting with >1 threads
    // were due to rand() not being thread safe. Each thread was calling rand() and
    // getting either the same number, numbers in sequence, or numbers too similar
    // and the output was looking as if we had samplesPerPixel / numTreads samples
    // TODO: Explore better random number generators

    // This ended up being the fastest solution. See commented out sections
    // for "proper" RN generation. I was probably using it wrong, but it was
    // still cutting performance in half
    //srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    double u = 0;
    double v = 0;
    double height = r_height->getValue();
    double width = r_width->getValue();
    Ray primaryRay;
    Vec3 pixelColor = Vec3();

    for (int i = 0; i < width; i++)
    {
        pixelColor.e[0] = 0;
        pixelColor.e[1] = 0;
        pixelColor.e[2] = 0;

        if (samples < 2)
        {
            u = i / width;
            v = scanLine / height;
            // Transform point in screenspace to world space
            // TODO: Remove this statement from here
            // TODO: Check if calling getRay is causing performance issues
            primaryRay = _pCamera->getRay(u, v);

            // Accumulate Pixel Color
            //pixelColor += shootRay(primaryRay, _pScene, 0);
            pixelColor += iterativeShootRay(primaryRay, _pScene);
        }
        else
        {
            // Take multiple samples per pixel
            // TODO: Detailed Profiling Data
            for (int s = 0; s < samples; s++)
            {
                // Compute both the U and V vectors for this point in screenspace
                //u = (i + random.getRandom(0.0, 1.0)) / width;
                //v = (scanLine + random.getRandom(0.0, 1.0)) / height;
                // FIXME: Remove this after the test
                u = i / width;
                v = scanLine / height;

                // Transform point in screenspace to world space
                // TODO: Remove this statement from here
                primaryRay = _pCamera->getRay(u, v);

                // Accumulate Pixel Color
                //pixelColor += shootRay(primaryRay, _pScene, 0);
                pixelColor += iterativeShootRay(primaryRay, _pScene);
            }
        }

        // Write to RenderTarget
        // This may not be the best way to do this. It might make sense to have n number of render
        // targets for n threads or at least have some sort of buffer system so we can combine
        // the accumulated output before we tonemap or modify the colorspace
        // TODO: Investigate
        line[i] = pixelColor;
        //_renderTarget.accumulate(scanLine, i, Vec3(pixelColor.x(), pixelColor.y(), pixelColor.z()));
    }
}

std::string RayTracer::renderingMode()
{
    if (r_renderingMode->getValue() == 1)
    {
        return "Sample Distribution";
    }
    else if (r_renderingMode->getValue() == 2)
    {
        return "Scan Line Interleave";
    }

    return "UNKNOWN";
}