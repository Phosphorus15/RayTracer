//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_HITABLE_LIST_H
#define RAYTRACER_HITABLE_LIST_H

#include "hitable.h"
#include <vector>
#include <memory>
#include <climits>

class hitable_list {
public:
    std::vector<std::unique_ptr<hitable>> list;

    bool hit(ray &r, float t_min, float t_max, hit_record &record) {
        hit_record temp;
        bool hit_any = false;
        float closest = t_max;
        for (auto &hit : list) {
            if (hit->hit(r, t_min, closest, temp)) { // including closest check
                record = temp;
                closest = temp.t;
                hit_any = true;
            }
        }
        return hit_any;
    }

};

#endif //RAYTRACER_HITABLE_LIST_H
