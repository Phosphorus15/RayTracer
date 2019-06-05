#include <cmath>

#include <iostream>
#include <cfloat>
#include <random>
#include <thread>
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
const int nx = 1920, ny = 1080, ns = 1000, nt = 12;
camera camera({1, 1.2, -.2}, {0, 0, -1}, 90, float(nx) / float(ny));

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

unsigned char *data;

void separate_job(int tnx, int tny, int tns, int from, int to) {
    uniform_random random;
    for (int j = from; j < to; j++) {
        for (int i = 0; i < tnx; i++) {
            vec3 mean{0, 0, 0};
            for (int s = 0; s < tns; s++) {
                float u = float(i + random()) / float(tnx);
                float v = float(j + random()) / float(tny);
                auto sample = camera.get_ray(u, v);
                mean = mean + color(sample);
            }
            mean = mean / float(tns);
            mean = vec3{std::sqrt(mean[0]), std::sqrt(mean[1]), std::sqrt(mean[2])};
            int ir = int(255.99 * mean.r());
            int ig = int(255.99 * mean.g());
            int ib = int(255.99 * mean.b());
            data[_calc_pixel(i, (tny - j - 1), tnx)] = ir;
            data[_calc_pixel(i, (tny - j - 1), tnx) + 1] = ig;
            data[_calc_pixel(i, (tny - j - 1), tnx) + 2] = ib;
        }
    }
    std::cout << "thread " << std::this_thread::get_id() << " finished" << std::endl;
}

int main() {
    clock_t start = clock();
    list.list.push_back(std::make_unique<sphere>(vec3{0, 0, -1}, 0.5, new lambertian({.5, .9, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{0, -.1, -.1}, 0.3, new metal({.5, .5, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{-1, 0, -1}, 0.5, new metal({.5, .5, .5}, .2)));
    list.list.push_back(std::make_unique<sphere>(vec3{1, 0, -1}, 0.5, new dielectrical(1.5)));
    list.list.push_back(std::make_unique<sphere>(vec3{.6, 1, -1.3}, 0.5, new metal({.3, .3, .5})));
    list.list.push_back(std::make_unique<sphere>(vec3{0, -100.5, -1}, 100, new lambertian({.5, .5, .5})));
    int ppt = ny / nt;
    data = new unsigned char[nx * ny * 3];
    std::vector<std::thread> threads;
    for (int i = 0; i < nt; i++) {
        std::thread t(separate_job, nx, ny, ns, ppt * i, ppt * (i + 1));
        std::cout << "start thread(id = " << t.get_id() << ")" << std::endl;
        threads.push_back(std::move(t));
    }
    for (auto &t : threads) {
        std::cout << "join" << std::endl;
        if (t.joinable()) t.join();
    }
    stbi_write_png("test.png", nx, ny, 3, data, nx * 3);
    std::cout << std::flush;
    clock_t end = clock();
    std::cout << "clock cost : " << (end - start) << std::endl;
    return 0;
}
