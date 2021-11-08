#pragma once

#include <iostream>
#include <cmath>

struct vec3;

vec3 operator*(const vec3& vec, const double& x);
vec3 operator*(const double& x, const vec3& vec);
vec3 operator/(const vec3& vec, const double& x);
vec3 operator/(const double& x, const vec3& vec);

struct vec3 {
    vec3(): e{0, 0, 0} {}
    vec3(double x, double y, double z): e{x,y,z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    vec3 unit_vec() { if (this->length() > 0) return *this / this->length();
                      else return vec3(0.0, 0.0, 0.0); }

    double length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }

    friend std::ostream& operator<<(std::ostream& out, const vec3& v);

    private:
        double e[3];
};

std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << int(v.x()) << ' ' << int(v.y()) << ' ' << int(v.z());
}

vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

vec3 operator-(const vec3& a, const vec3& b) {
    return a + (-b);
}

vec3 operator*(const vec3& vec, const double& x) { return vec3(x*vec.x(), x*vec.y(), x*vec.z()); }
vec3 operator*(const double& x, const vec3& vec) { return vec3(x*vec.x(), x*vec.y(), x*vec.z()); }
vec3 operator/(const vec3& vec, const double& x) { return vec * (1/x); }
vec3 operator/(const double& x, const vec3& vec) { return vec * (1/x); }

using color = vec3;
using point3 = vec3;
