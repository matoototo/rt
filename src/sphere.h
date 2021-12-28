#pragma once

#include "object.h"
#include "vec3.h"
#include "ray.h"

struct Sphere : Object {
    Sphere(const point3& center, const float& radius, const Props& props):
            Object(props), center(center), radius(radius) {}

    float hit(const ray& r) const;
    vec3 normal(const point3& hp) const;

    point3 center;
    float radius;
};


// R = ray, C = sphere centre
// (R-C)(R-C) = r^2
// R^2 - 2RC + C^2 - r^2 = 0
// (A+Bx)^2 - 2C(A + Bx) + C^2 - r^2 = 0
// (Bx)^2 + (2AB - 2BC)x + A^2 + C^2 - r^2 - 2AC = 0
// (Bx)^2 + 2B(A-C)x + (A-C)^2 - r^2 = 0
inline float Sphere::hit(const ray& r) const {
    vec3 oc = r.orig - this->center;
    float a = r.dir * r.dir;
    float b = 2 * r.dir * oc;
    float c = oc*oc - this->radius * this->radius;

    float disc = (b*b - 4*a*c);

    if (disc < 0) return -1;
    return (-b - sqrt(disc))/(2*a);
}

inline vec3 Sphere::normal(const point3& hp) const {
    return (hp - this->center).unit_vec();
}

