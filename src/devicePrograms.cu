// ======================================================================== //
// Copyright 2018-2019 Ingo Wald                                            //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include <optix_device.h>

#include "./renderer/ConstantBuffer.h"
#include "math.h"
#include "random.h"

using namespace gdt;

struct PerRayData
{
    vec3f       direction = vec3f(0.0f);
    vec3f       origin    = vec3f(0.0f);
    vec3f       color     = vec3f(0.0f);
    vec3f       emitted   = vec3f(0.0f);
    bool        done      = false;
};

/*! launch parameters in constant memory, filled in by optix upon
      optixLaunch (this gets filled in from the buffer we pass to
      optixLaunch) */
extern "C" __constant__ ConstantBuffer optixLaunchParams;

// for this simple example, we have a single ray type
enum { SURFACE_RAY_TYPE = 0, RAY_TYPE_COUNT };

static __forceinline__ __device__
void* unpackPointer(uint32_t i0, uint32_t i1)
{
    const uint64_t uptr = static_cast<uint64_t>(i0) << 32 | i1;
    void* ptr = reinterpret_cast<void*>(uptr);
    return ptr;
}

static __forceinline__ __device__
void  packPointer(void* ptr, uint32_t& i0, uint32_t& i1)
{
    const uint64_t uptr = reinterpret_cast<uint64_t>(ptr);
    i0 = uptr >> 32;
    i1 = uptr & 0x00000000ffffffff;
}

template<typename T>
static __forceinline__ __device__ T* getPRD()
{
    const uint32_t u0 = optixGetPayload_0();
    const uint32_t u1 = optixGetPayload_1();
    return reinterpret_cast<T*>(unpackPointer(u0, u1));
}

static __forceinline__ __device__ vec3f reflectRay(const vec3f& rayDir, const vec3f& normal)
{
    //return (rayDir - 2.0f * dot(rayDir / length(rayDir), normal) * normal) + .0f * rnd(prd.seed);
    return (rayDir - 2.0f * dot(rayDir / length(rayDir), normal) * normal);
}

static __forceinline__ __device__ vec3f calculateNormals(const TriangleMeshSBTData& sbtData, const vec3i& index, const vec2f& uv)
{
    // Verts
    const vec3f& v0 = sbtData.vertex[index.x];
    const vec3f& v1 = sbtData.vertex[index.y];
    const vec3f& v2 = sbtData.vertex[index.z];
    
    vec3f N;
    
    if (sbtData.normals)
    {
        N = (1.0f - uv.u - uv.v)
            * sbtData.normals[index.x]
            + uv.u * sbtData.normals[index.y]
            + uv.v * sbtData.normals[index.z];
    }
    else
    {
        N = normalize(cross(v1 - v0, v2 - v0));
    }

    N = normalize(N);

    return N;
}

static __forceinline__ __device__ vec3f mapTextures(const TriangleMeshSBTData& sbtData, const vec3i& index, const vec2f& uv)
{
    // Diffuse color and textures
    vec3f diffuseColor = sbtData.color;

    if (sbtData.hasTexture && sbtData.texcoords)
    {
        const vec2f tc = (1.f - uv.u - uv.v)
            * sbtData.texcoords[index.x]
            + uv.u * sbtData.texcoords[index.y]
            + uv.v * sbtData.texcoords[index.z];

        vec4f fromTexture = tex2D<float4>(sbtData.texture, tc.x, tc.y);
        diffuseColor *= (vec3f)fromTexture;
    }

    return diffuseColor;
}

//------------------------------------------------------------------------------
// closest hit and anyhit programs for radiance-type rays.
//
// Note eventually we will have to create one pair of those for each
// ray type and each geometry type we want to render; but this
// simple example doesn't use any actual geometries yet, so we only
// create a single, dummy, set of them (we do have to have at least
// one group of them to set up the SBT)
//------------------------------------------------------------------------------

extern "C" __global__ void __closesthit__radiance()
{
    const TriangleMeshSBTData& sbtData
        = *(const TriangleMeshSBTData*)optixGetSbtDataPointer();

    PerRayData& prd = *(PerRayData*)getPRD<vec3f>();

    // Get Primitive Index Information
    const int   primitiveIndex = optixGetPrimitiveIndex();
    const vec3i index = sbtData.index[primitiveIndex];

    // Get UVs for this Primitive
    const vec2f uv(optixGetTriangleBarycentrics().x, optixGetTriangleBarycentrics().y);

    // Compute Interpolated Per-Vertex Normals with Per-Face normals as fallback
    vec3f N = calculateNormals(sbtData, index, uv);

    // Diffuse color and textures
    vec3f diffuseColor = mapTextures(sbtData, index, uv);

    // Get ray direction and origin from Optix
    const vec3f rayDir = optixGetWorldRayDirection();
    const vec3f origin = optixGetWorldRayOrigin();

    // Compute next origin (point of impact) and store it in our
    // per-ray data struct for the next trace
    const vec3f P = origin + (optixGetRayTmax() * rayDir);
    prd.origin = P;

    // Compute the new direction and radiance transfer based
    // of of the material on this primitive
    const float cosDN = 0.2f + .8f * fabsf(dot(rayDir, N)); // phong lighting?
    prd.color = cosDN * diffuseColor;
    prd.done = true; // Finished Tracing

    // Saved for metal material
    //prd.direction = reflectRay(rayDir, N);
}

extern "C" __global__ void __anyhit__radiance()
{ /*! for this simple example, this will remain empty */
}

//------------------------------------------------------------------------------
// miss program that gets called for any ray that did not have a
// valid intersection
//
// as with the anyhit/closest hit programs, in this example we only
// need to have _some_ dummy function to set up a valid SBT
// ------------------------------------------------------------------------------

extern "C" __global__ void __miss__radiance()
{
    PerRayData& prd = *(PerRayData*)getPRD<vec3f>();

    // Set to constant black as background color and mark that
    // we should stop tracing this secondary ray
    prd.color = vec3f(0.0f);
    prd.done = true;
}

//------------------------------------------------------------------------------
// ray gen program - the actual rendering happens in here
//------------------------------------------------------------------------------
extern "C" __global__ void __raygen__renderFrame()
{
    // This is our perRay data structure similar to HitRecord
    // in software mode
    PerRayData perRayData;
    
    // compute a test pattern based on pixel ID
    const int ix = optixGetLaunchIndex().x;
    const int iy = optixGetLaunchIndex().y;

    // Because we are accumulating, init finalColor to 1.0f
    vec3f finalColor(0.0f);

    const auto& camera = optixLaunchParams.camera;

    // normalized screen plane position, in [0,1]^2
    const vec2f screen(vec2f(ix + .5f, iy + .5f)
        / vec2f(optixLaunchParams.frame.size));

    vec3f rayOrigin = camera.position;

    // the values we store the PRD pointer in:
    uint32_t u0, u1;
    packPointer(&perRayData, u0, u1);

    // generate ray direction
    vec3f rayDirection = normalize(camera.direction
        + (screen.x - 0.5f) * camera.horizontal
        + (screen.y - 0.5f) * camera.vertical);

    // Quick Hack for multi sampling
    int numberOfSamples = 10;
    int depth = 0;
    int maxBounces = 3;

    // While we are not done yet, spawn rays!
    while (!perRayData.done && depth < maxBounces)
    {
        optixTrace(optixLaunchParams.traversable,
            rayOrigin,
            rayDirection,
            0.01f,    // tmin
            1e16f,  // tmax
            0.0f,   // rayTime
            OptixVisibilityMask(255),
            OPTIX_RAY_FLAG_DISABLE_ANYHIT,//OPTIX_RAY_FLAG_NONE,
            SURFACE_RAY_TYPE,             // SBT offset
            RAY_TYPE_COUNT,               // SBT stride
            SURFACE_RAY_TYPE,             // missSBTIndex 
            u0, u1);

        // Want to accumulate the color here per trace
        finalColor += perRayData.color;

        // Set new ray direction and origin from previous ray intersection
        rayOrigin    = perRayData.origin;
        rayDirection = perRayData.direction;

        // Increase the depth, or number of secondary relections allowed before
        // we have to cut off the trace
        depth++;
    }

    // Write RGB Values
    const int r = int(255.99f * finalColor.x);
    const int g = int(255.99f * finalColor.y);
    const int b = int(255.99f * finalColor.z);

    // convert to 32-bit rgba value (we explicitly set alpha to 0xff
    // to make stb_image_write happy ...
    const uint32_t rgba = 0xff000000
        | (r << 0) | (g << 8) | (b << 16);

    // and write to frame buffer ...
    const uint32_t fbIndex = ix + iy * optixLaunchParams.frame.size.x;
    optixLaunchParams.frame.colorBuffer[fbIndex] = rgba;
}