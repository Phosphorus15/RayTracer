cmake_minimum_required(VERSION 3.14)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp vec3.h ray.h hitable.h sphere.h hitable_list.h camera.h material.h)