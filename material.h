//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

class material;

#include "hitable.h"

vec3 random_unit_sphere() {
    static uniform_random random;
    vec3 point;
    do {
        point = vec3{random(), random(), random()} * 2.0 - vec3{1, 1, 1};
    } while (point.squared_length() >= 1.0);
    return point;
}

class material {
public:
    virtual bool scatter(ray &r, const hit_record &record, vec3 &attenuation, ray &scattered) = 0;
};

class lambertian : public material {
public:
    vec3 albedo;

    lambertian(vec3 albedo) : albedo(albedo) {}

    bool scatter(ray &r, const hit_record &record, vec3 &attenuation, ray &scattered) override {
        auto target = record.p + record.norm + random_unit_sphere();
        scattered = ray(record.p, target - record.p);
        attenuation = albedo;
        return true;
    }

};

class metal : public material {
public:
    vec3 albedo;
    float fuzz = 0;

    metal(vec3 albedo, float fuzz = 0) : albedo(albedo), fuzz(fuzz) {}

    bool scatter(ray &r, const hit_record &record, vec3 &attenuation, ray &scattered) override {
        auto direction =
                r.direction().make_unit_vector() - record.norm * dot(r.direction().make_unit_vector(), record.norm) * 2
                + random_unit_sphere() * fuzz;
        scattered = ray(record.p, direction);
        attenuation = albedo;
        return (dot(direction, record.norm) > 0);
    }
};

// n sin a = n' sin a'

class dielectrical : public material {
public:
    float ri;

    dielectrical(float ri) : ri(ri) {}

    bool calc_refract(vec3 u, vec3 &n, float i_to_t, vec3 &refracted) {
        float cosine = dot(u.make_unit_vector(), n.make_unit_vector());
        float discriminant = 1.0 - (i_to_t * i_to_t) * (1.0 - cosine * cosine); // cosine t
        if (discriminant > 0) {
            refracted =
                    u.make_unit_vector() * i_to_t - n.make_unit_vector() * (cosine * i_to_t + std::sqrt(discriminant));
            return true;
        }
        return false;
    }

    float schlick(float cosine, float ref) { // FIXME needs paper support
        float r0 = (1 - ref) / (1 + ref);
        r0 = r0 * r0;
        return r0 * (1 - r0) * pow(1 - cosine, 5);
    }

    bool scatter(ray &r, const hit_record &record, vec3 &attenuation, ray &scattered) override {
        static uniform_random random;
        attenuation = {1, 1, 1};
        vec3 real_norm;
        float ref;
        float cosine;
        if (dot(r.direction(), record.norm) > 0) {
            real_norm = -record.norm;
            ref = ri;
            cosine = ref * dot(r.direction(), record.norm) / r.direction().length();
        } else {
            real_norm = record.norm;
            ref = 1 / ri;
            cosine = - dot(r.direction(), record.norm) / r.direction().length();
        }
        float reflect_prob;
        vec3 refracted;
        if (calc_refract(r.direction(), real_norm, ref, refracted)) {
            reflect_prob = schlick(cosine, ri);
        } else {
            reflect_prob = 1.0;
        }
        if(random() < reflect_prob) {
            auto reflect =
                    r.direction().make_unit_vector() -
                    record.norm * dot(r.direction().make_unit_vector(), record.norm) * 2;
            scattered = ray(record.p, refracted);
            return false;
        } else {
            scattered = ray(record.p + refracted * 0.01, refracted);
        }
        return true;
    }
};


#endif //RAYTRACER_MATERIAL_H
