#pragma once

#include <iostream>
#include <cmath>

#include "util.h"

struct vec3;

vec3 operator*(const vec3& vec, const float& x);
vec3 operator*(const float& x, const vec3& vec);
float operator*(const vec3& v1, const vec3& v2);
vec3 operator/(const vec3& vec, const float& x);
vec3 operator/(const float& x, const vec3& vec);
inline vec3 operator+(const vec3& a, const vec3& b);

struct vec3 {
    constexpr vec3(): e{0, 0, 0} {}
    constexpr vec3(float x, float y, float z): e{x,y,z} {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    vec3 operator*=(const float& x) { e[0] *= x; e[1] *= x; e[2] *= x; return *this; }
    vec3 unit_vec() const { if (this->length() > 0) return *this / this->length();
                            else return vec3(0.0, 0.0, 0.0); }

    float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }

    static vec3 rand();

    friend std::ostream& operator<<(std::ostream& out, const vec3& v);

    float e[3];
    // private:
};


inline vec3 vec3::rand() {
    return vec3(rdbl(), rdbl(), rdbl());
}

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

inline vec3 operator-(const vec3& a, const vec3& b) {
    return a + (-b);
}

inline vec3 hadamard(const vec3& v1, const vec3& v2) { return vec3(v1.x()*v2.x(), v1.y()*v2.y(), v1.z()*v2.z()); }
inline vec3 cross(const vec3& v1, const vec3& v2) { return vec3(v1.y()*v2.z() - v1.z()*v2.y(),
                                                                v1.z()*v2.x() - v1.x()*v2.z(),
                                                                v1.x()*v2.y() - v1.y()*v2.x()); }

inline vec3 operator*(const vec3& vec, const float& x) { return vec3(x*vec.x(), x*vec.y(), x*vec.z()); }
inline vec3 operator*(const float& x, const vec3& vec) { return vec3(x*vec.x(), x*vec.y(), x*vec.z()); }
inline float operator*(const vec3& v1, const vec3& v2) { return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z(); }
inline vec3 operator/(const vec3& vec, const float& x) { return vec * (1/x); }
inline vec3 operator/(const float& x, const vec3& vec) { return vec * (1/x); }
inline vec3 operator+(const vec3& vec, const float& x) { return vec3(x+vec.x(), x+vec.y(), x+vec.z()); }
inline vec3 operator+(const float& x, const vec3& vec) { return vec3(x+vec.x(), x+vec.y(), x+vec.z()); }
inline vec3& operator+=(vec3& v1, const vec3& v2) { v1 = v1 + v2; return v1; }


using color = vec3;
using point3 = vec3;
