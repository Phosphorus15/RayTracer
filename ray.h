//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

struct uniform_random {
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

    uniform_random() {
        mt = std::mt19937(time(nullptr));
    }

    float operator()() {
        return dist(mt);
    }
};

class ray {
public:
    vec3 a, b;

    ray() = default;

    ray(const vec3 &a, const vec3 &b) : a(a), b(b) {
    }

    vec3 origin() {
        return a;
    }

    vec3 direction() {
        return b;
    }

    vec3 point_at(float t) {
        return a + b * t;
    }
};

#endif //RAYTRACER_RAY_H
