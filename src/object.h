#pragma once

#include "ray.h"

struct Object {
    Object(double const& g, color const& c): glow(g), obj_color(c) {}

    virtual double hit(const ray& r) const = 0;
    virtual vec3 normal(const point3& hp) const = 0;
    ray scatter(const point3& hp, const ray& r_in) const;

    color obj_color;
    double glow;
};

inline ray Object::scatter(const point3& hp, const ray& r_in) const {
    vec3 normal = this->normal(hp);
    if (r_in.dir * normal > 0) normal = -normal;
    return ray(hp, normal.unit_vec() + vec3::rand().unit_vec());
}
