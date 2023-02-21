cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(raytracer VERSION 0.1.0)

if (APPLE)
    SET(CMAKE_CXX_FLAGS "-g")
    set (CMAKE_CXX_STANDARD 17)
elseif (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /EHsc")
else()
    SET(CMAKE_CXX_FLAGS "-Wall -pthread -g")
    set (CMAKE_CXX_STANDARD 17)
endif()

INCLUDE_DIRECTORIES(SYSTEM ./external /opt/homebrew/include /usr/include/catch2)

file(GLOB_RECURSE raytracer_src
    "./src/*.cpp"
    "./external/*.cpp" # Compile all external cpp files
    "./external/*.c" # Compile all external c files
)

file(GLOB_RECURSE raytracer_tests
    "./tests/*.test.cpp")

find_package(nlohmann_json REQUIRED)
find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

# Main Executable
add_executable(raytracer ${raytracer_src})

target_compile_definitions(raytracer PRIVATE)
target_include_directories(raytracer PRIVATE Catch2::Catch2 fmt::fmt spdlog::spdlog)
target_link_libraries(raytracer Catch2::Catch2 fmt::fmt spdlog::spdlog ${CMAKE_DL_LIBS})

# Test Executable
add_executable(raytracerT ${raytracer_tests})
target_include_directories(raytracerT PRIVATE Catch2::Catch2)
target_link_libraries(raytracerT Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(raytracerT)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


# IF(APPLE)
# add_custom_command(TARGET raytracer
#         POST_BUILD
#         COMMAND /bin/sh ../tools/compileShaders.sh
#         )
# ELSEIF (WIN32)
# ELSE()
# #add_custom_command(TARGET runic
# #        POST_BUILD
# #        COMMAND /bin/sh ../tools/compileShadersGNU.sh
# #        )
# ENDIF()