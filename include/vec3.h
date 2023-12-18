#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "global.h"

class vec3 {
public:
    vec3() {};
    vec3(float e0, float e1, float e2) {e[0] = e0; e[1] = e1; e[2] = e2; }
    inline float x() { return e[0]; }
    inline float y() { return e[1]; }
    inline float z() { return e[2]; }
    inline float r() { return e[0]; }
    inline float g() { return e[1]; }
    inline float b() { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    inline float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    inline void make_unit_vector();

    float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}
inline std::ostream& operator<<(std::ostream &os, vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}
inline void vec3::make_unit_vector() {
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}
inline vec3 operator+(const vec3 v1, const vec3 v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline vec3 operator-(const vec3 v1, const vec3 v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline vec3 operator*(const vec3 v1, const vec3 v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline vec3 operator/(const vec3 v1, const vec3 v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}
inline vec3 operator*(const vec3 &v, float t) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); 
}
inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); 
}
inline vec3 operator/(const vec3 &v, float t) {
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}
inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (- v1.e[0] * v2.e[2] + v1.e[2] * v2.e[0]),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3& vec3::operator+=(const vec3 &v2) {
    e[0] += v2.e[0]; e[1] += v2.e[1]; e[2] += v2.e[2];
    return *this;
}
inline vec3& vec3::operator-=(const vec3 &v2) {
    e[0] -= v2.e[0]; e[1] -= v2.e[1]; e[2] -= v2.e[2];
    return *this;
}
inline vec3& vec3::operator*=(const vec3 &v2) {
    e[0] *= v2.e[0]; e[1] *= v2.e[1]; e[2] *= v2.e[2];
    return *this;
}
inline vec3& vec3::operator/=(const vec3 &v2) {
    e[0] /= v2.e[0]; e[1] /= v2.e[1]; e[2] /= v2.e[2];
    return *this;
}
inline vec3& vec3::operator*=(const float t) {
    e[0] *= t; e[1] *= t; e[2] *= t;
    return *this;
}
inline vec3& vec3::operator/=(const float t) {
    e[0] /= t; e[1] /= t; e[2] /= t;
    return *this;
}
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}
inline vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2*vec3(get_random_float(), get_random_float(), get_random_float()) - vec3(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0);
    return p;
}

inline vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2 * vec3(get_random_float(), get_random_float(), 0) - vec3(1.0, 1.0, 0.0);
    } while (dot(p, p) >= 1.0);
    return p;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - dt * dt);
    if(discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else return false;
}

#endif