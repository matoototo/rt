#pragma once

#include "vec3.h"

struct ray {
    ray(): orig(0, 0, 0), dir(0, 0, 0) {}
    ray(const point3& o, const vec3& d): orig(o), dir(d) {}

    point3 at(const float& x) const { return orig + x*dir; }

    point3 orig;
    vec3 dir;
};
