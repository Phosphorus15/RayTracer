#include <cmath>

//
// Created by Phosphorus15 on 2019/6/4.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>

class vec3 {
public:
    vec3() = default;

    vec3(float e0, float e1, float e2);

    inline float x() const { return e[0]; }

    inline float y() const { return e[1]; }

    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }

    inline float g() const { return e[1]; }

    inline float b() const { return e[2]; }

    inline const vec3 &operator+() const { return *this; }

    inline vec3 operator-() const {
        return {-e[0], -e[1], -e[2]};
    }

    inline vec3 operator/(float v) {
        return {e[0] / v, e[1] / v, e[2] / v};
    }

    inline vec3 operator*(float v) const {
        return {e[0] * v, e[1] * v, e[2] * v};
    }

    inline float operator[](int i) const { return e[i]; }

    inline float &operator[](int i) { return e[i]; };

    inline float length() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }

    inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline vec3 make_unit_vector() {
        auto len = length();
        return {e[0] / len, e[1] / len, e[2] / len};
    }

    friend inline std::ostream& operator<<(std::ostream& out,const vec3& vec) {
        out << "vec3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
        return out;
    }

    float e[3]{};
};

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]};
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]};
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]};
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return {v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]};
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {
            v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
            v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
            v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
    };
}

vec3::vec3(float e0, float e1, float e2) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
}

#endif //RAYTRACER_VEC3_H
