#include <cmath>

#include <iostream>
#include <cfloat>
#include <random>
#include <ctime>
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define _calc_pixel(i, j, w) (i + j * w) * 3

hitable_list list;
camera camera({1, 1, -.2}, {0, 0, -1},90, 2);

vec3 color(ray r, int depth = 0) {
    hit_record record;
    if (list.hit(r, 0.0, FLT_MAX, record)) {
        vec3 attenuation;
        ray scattered;
        if (depth < 50 && record.mat->scatter(r, record, attenuation, scattered)) {
            return color(scattered, depth + 1) * attenuation;
        }
    }
    vec3 direction = r.direction().make_unit_vector();
    float t = .5 * (direction.y() + 1.0);
    return vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(.5, .7, 1.0) * t;
}

int main() {
    uniform_random random;
    list.list.push_back(std::make_unique<sphere>(vec3{0, 0, -1}, 0.5, new lambertian({.5, .9, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{0, -.1, -.1}, 0.3, new metal({.5, .5, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{-1, 0, -1}, 0.5, new metal({.5, .5, .5}, .2)));
    list.list.push_back(std::make_unique<sphere>(vec3{1, 0, -1}, 0.5, new dielectrical(1.5)));
    list.list.push_back(std::make_unique<sphere>(vec3{.6, 1, -1.3}, 0.5, new metal({.3, .3, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{0, -100.5, -1}, 100, new lambertian({.5, .5, .5})));
    int nx = 480, ny = 240, ns = 500;
    auto *data = new unsigned char[nx * ny * 3];
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            vec3 mean{0, 0, 0};
            for (int s = 0; s < ns; s++) {
                float u = float(i + random()) / float(nx);
                float v = float(j + random()) / float(ny);
                auto sample = camera.get_ray(u, v);
                mean = mean + color(sample);
            }
            mean = mean / float(ns);
            mean = vec3{std::sqrt(mean[0]), std::sqrt(mean[1]), std::sqrt(mean[2])};
            int ir = int(255.99 * mean.r());
            int ig = int(255.99 * mean.g());
            int ib = int(255.99 * mean.b());
            data[_calc_pixel(i, (ny - j - 1), nx)] = ir;
            data[_calc_pixel(i, (ny - j - 1), nx) + 1] = ig;
            data[_calc_pixel(i, (ny - j - 1), nx) + 2] = ib;
        }
    }
    stbi_write_png("test.png", nx, ny, 3, data, nx * 3);
    std::cout << std::flush;
    return 0;
}
