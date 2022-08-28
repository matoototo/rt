#pragma once

#include "ray.h"
#include "vec3.h"

float deg_to_rad(const float& deg);

struct Camera {
    Camera(const point3& origin, const point3& at, const float& aspect_ratio, const float& fov):
    origin(origin), at(at), aspect_ratio(aspect_ratio), fov(fov) {
        calculate_cam();
    }

    ray get_ray(const float& x, const float& y) const;
    void move(const vec3& dir);
    void rotate(const vec3& dir);
    void calculate_cam();
    point3 origin, at;
    vec3 bl, lim_x, lim_y;
    float aspect_ratio, fov;
};


inline ray Camera::get_ray(const float& x, const float& y) const {
    return ray(origin, bl + lim_x*x + lim_y*y - origin);
}

inline void Camera::calculate_cam() {
    float fov_height = tan(deg_to_rad(fov/2));
    float vph = 2*fov_height;
    float vpw = vph * aspect_ratio;

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 cam_dir = (origin-at).unit_vec();
    vec3 ort_dir = cross(up, cam_dir).unit_vec();
    vec3 third_dir = cross(cam_dir, ort_dir).unit_vec();

    lim_x = vpw * ort_dir;
    lim_y = vph * third_dir;
    bl = origin - this->lim_x/2 - this->lim_y/2 - cam_dir;
}

inline void Camera::move(const vec3& dir) {
    origin += dir;
    at += dir;
    calculate_cam();
}

inline float deg_to_rad(const float& deg) {
    return tan(deg * M_PI / 180.f);
}
