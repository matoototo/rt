#pragma once

#include "ray.h"
#include "vec3.h"

float deg_to_rad(const float& deg);

struct Camera {
    Camera(const point3& origin, const point3& at, const float& aspect_ratio, const float& focal_length, const float& fov):
    origin(origin), at(at), aspect_ratio(aspect_ratio), focal_length(focal_length), fov(fov) {
        float fov_height = tan(deg_to_rad(fov)/2);
        float vph = 2*fov_height;
        float vpw = vph * aspect_ratio;
        lim_x = vec3(vpw, 0, 0);
        lim_y = vec3(0, vph, 0);
        bl = origin - this->lim_x/2 - this->lim_y/2 - vec3(0, 0, focal_length);
    }

    ray get_ray(const float& x, const float& y) const;
    point3 origin, at;
    vec3 up, bl, lim_x, lim_y;
    float aspect_ratio, fov, focal_length;
};

inline ray Camera::get_ray(const float& x, const float& y) const {
    return ray(origin, bl + lim_x*x + lim_y*y - origin);
}

inline float deg_to_rad(const float& deg) {
    return tan(deg * M_PI / 180.f);
}
