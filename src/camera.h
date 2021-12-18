#pragma once

#include "vec3.h"

struct Camera {
    Camera(const point3& origin, const float& vpw, const float& vph, const float& focal_length):
        origin(origin), vpw(vpw), vph(vph), focal_length(focal_length) {}

    point3 origin;

    float vpw;
    float vph;
    float focal_length = 1.0;

};
