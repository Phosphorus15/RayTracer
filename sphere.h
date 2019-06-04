//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hitable.h"

class sphere : public hitable {
public:
    vec3 center;
    float radius = 0;
    material* mat;

    sphere() = default;

    sphere(vec3 center, float r, material* mat) : center(center), radius(r), mat(mat) {}

    bool hit(ray &r, float t_min, float t_max, hit_record &record) const override;
};

bool sphere::hit(ray &r, float t_min, float t_max, hit_record &record) const {
    vec3 a = r.a;
    vec3 b = r.b;
    vec3 c = center;
    float B = dot(b, (a - c) * 2);
    float A = dot(b, b);
    float C = dot(a - c, a - c) - radius * radius;
    float determinant = B * B - 4 * A * C;
    if (determinant >= 0) {
        float t1 = (-B - std::sqrt(determinant)) / (A * 2.0);
        if (t1 > t_min && t1 < t_max) { // FIXME possible stuck here if allow equivalence
            record.t = t1;
            record.p = r.point_at(t1);
            record.norm = (record.p - center) / radius;
            record.mat = this->mat;
            return true;
        }
        float t2 = (-B + std::sqrt(determinant)) / (A * 2.0);
        if (t2 > t_min && t2 < t_max) {
            record.t = t2;
            record.p = r.point_at(t2);
            record.norm = (record.p - center) / radius;
            record.mat = this->mat;
            return true;
        }
    }
    return false;
}

#endif //RAYTRACER_SPHERE_H
