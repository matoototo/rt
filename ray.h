#pragma once

#include "vec3.h"

struct ray {
    ray(): orig(0, 0, 0), dir(0, 0, 0) {}
    ray(const point3& o, const point3& d): orig(o), dir(d) {}

    point3 orig;
    point3 dir;
};
