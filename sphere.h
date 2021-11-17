#pragma once

#include "ray.h"

struct Sphere {
    Sphere(const point3& center, const double& radius, const color& color): center(center), radius(radius), color(color) {}

    bool is_hit(const ray& r) const;

    color color;
    point3 center;
    double radius;
};


// R = ray, C = sphere centre
// (R-C)(R-C) = r^2
// R^2 - 2RC + C^2 - r^2 = 0
// (A+Bx)^2 - 2C(A + Bx) + C^2 - r^2 = 0
// (Bx)^2 + (2AB - 2BC)x + A^2 + C^2 - r^2 - 2AC = 0
// (Bx)^2 + 2B(A-C)x + (A-C)^2 - r^2 = 0
bool Sphere::is_hit(const ray& r) const {
    vec3 oc = r.orig - this->center;
    double a = r.dir * r.dir;
    double b = 2 * r.dir * oc;
    double c = oc*oc - this->radius * this->radius;
    return (b*b - 4*a*c) > 0;
}