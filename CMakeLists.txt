cmake_minimum_required(VERSION 3.15)
project(runic)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/config)
include_directories(src/includes)
include_directories(src/logging)
include_directories(src/materials)
include_directories(src/renderer)
include_directories(src/scene)
include_directories(src/shapes)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(eval
        src/config/Config.cpp
        src/config/Config.h
        src/config/CVar.cpp
        src/config/CVar.h
        src/confic/CVarDescriptor.h
        src/includes/DRand48.h
        src/includes/ply.cpp
        src/includes/ply.h
        src/includes/random.h
        src/includes/stb_image.h
        src/includes/stb_image_write.h
        src/includes/tiny_obj_loader.h
        src/logging/ConsoleLog.cpp
        src/logging/ConsoleLog.h
        src/logging/Log.h
        src/materials/Dielectric.cpp
        src/materials/Dielectric.h
        src/materials/DiffuseEmitter.cpp
        src/materials/DiffuseEmitter.h
        src/materials/Lambertian.cpp
        src/materials/Lambertian.h
        src/materials/Material.cpp
        src/materials/Material.h
        src/materials/MaterialFactory.cpp
        src/materials/MaterialFactory.h
        src/materials/Metal.cpp
        src/materials/Metal.h
        src/renderer/RayTracer.cpp
        src/renderer/RayTracer.h
        src/renderer/Renderer.cpp
        src/renderer/Renderer.h
        src/renderer/RenderTarget.cpp
        src/renderer/RenderTarget.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/shapes/Rect.cpp
        src/shapes/Rect.h
        src/shapes/Shape.h
        src/shapes/ShapeFactory.cpp
        src/shapes/ShapeFactory.h
        src/shapes/Sphere.cpp
        src/shapes/Sphere.h
        src/shapes/Triangle.cpp
        src/shapes/Triangle.h
        src/Camera.cpp
        src/Camera.h
        src/Color.cpp
        src/Color.h
        src/Component.cpp
        src/Component.h
        src/Core.h
        src/HitRecord.cpp
        src/HitRecord.h
        src/main.cpp
        src/Ray.cpp
        src/Ray.h
        src/Vec3.cpp
        src/Vec3.h
        src/Window.cpp
        src/Window.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
