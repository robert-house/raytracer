cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(raytracer VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

if (APPLE)
    SET(CMAKE_CXX_FLAGS "-g")
elseif (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /EHsc")
else()
    SET(CMAKE_CXX_FLAGS "-Wall -pthread -g")
endif()

INCLUDE_DIRECTORIES(SYSTEM /opt/homebrew/include)

file(GLOB_RECURSE raytracer_src
    "./src/*.cpp"
    "./external/*.c" # Compile all external c files
)

# Main Executable
add_executable(raytracer ${raytracer_src})

target_compile_definitions(raytracer PRIVATE)
target_include_directories(raytracer PRIVATE)
target_link_libraries(raytracer ${CMAKE_DL_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
