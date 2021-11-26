#pragma once

#include "ray.h"

struct Object {
    Object(double const& g, color const& c): glow(g), obj_color(c) {}

    virtual double hit(const ray& r) const = 0;
    virtual vec3 normal(const point3& hp) const = 0;

    color obj_color;
    double glow;
};
