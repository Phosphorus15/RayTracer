//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_HITABLE_H
#define RAYTRACER_HITABLE_H

#include "ray.h"

struct hit_record{
    float t = 0;
    vec3 p;
    vec3 norm;
    material* mat;
};

class hitable {
public:
    virtual bool hit(ray& r, float t_min, float t_max, hit_record& record) const = 0;
};

#endif //RAYTRACER_HITABLE_H
