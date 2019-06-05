//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "vec3.h"
#include "ray.h"

class camera {
public:
    vec3 lower_left, horizontal, vertical, origin;

    explicit camera(vec3 from, vec3 to, float v, float aspect_ratio, vec3 vup = vec3(0, 1, 0)) {
        float angle = v * M_PI / 180;
        float half_height = std::tan(angle / 2);
        float half_width = half_height * aspect_ratio;
        origin = from;
        vec3 z = (from - to).make_unit_vector();
        vec3 y = cross(vup, z).make_unit_vector();
        vec3 x = cross(z, y).make_unit_vector();
        lower_left = (-y) * half_width + (-x) * half_height - z + origin;
        horizontal = y * 2 * half_width;
        vertical = x * 2 * half_height;
    }

    ray get_ray(float u, float v) {
        return {origin, lower_left + horizontal * u + vertical * v - origin};
    }
};

#endif //RAYTRACER_CAMERA_H
