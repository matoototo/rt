#pragma once

#include "vec3.h"

struct Camera {
    Camera(const point3& origin, const double& vpw, const double& vph, const double& focal_length):
        origin(origin), vpw(vpw), vph(vph), focal_length(focal_length) {}

    point3 origin;

    double vpw;
    double vph;
    double focal_length = 1.0;

};
