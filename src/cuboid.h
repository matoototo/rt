#pragma once

#include <memory>
#include <limits>

#include "object.h"
#include "rectangle.h"


struct Cuboid : Object {
    Cuboid(const point3& o, const double& w, const double& h, const double& d, const color& color, const double& gl):
        Object(gl, color), orig(o), w(w), h(h), d(d) {
        faces.push_back(std::make_shared<Rectangle>(o + vec3{0, 0, 0}, -d, h, color, gl, x_side));
        faces.push_back(std::make_shared<Rectangle>(o + vec3{w, 0, 0}, -d, h, color, gl, x_side));
        faces.push_back(std::make_shared<Rectangle>(o + vec3{0, 0, 0}, w, -d, color, gl, y_side));
        faces.push_back(std::make_shared<Rectangle>(o + vec3{0, h, 0}, w, -d, color, gl, y_side));
        faces.push_back(std::make_shared<Rectangle>(o + vec3{0, 0, 0}, w, h, color, gl, z_side));
        faces.push_back(std::make_shared<Rectangle>(o + vec3{0, 0, d}, w, h, color, gl, z_side));
    }

    double hit(const ray& r) const;
    vec3 normal(const point3& hp) const;

    point3 orig;
    double w, h, d;
    std::vector<std::shared_ptr<Rectangle>> faces;
};

inline double Cuboid::hit(const ray& r) const {
    double t, t_min;
    t_min = std::numeric_limits<double>::max();

    for (auto& face : faces) {
        t = face->hit(r);
        if (t > 0 && t < t_min) t_min = t;
    }

    if (t_min < std::numeric_limits<double>::max()) return t_min;
    return -1;
}


inline vec3 Cuboid::normal(const point3& hp) const {
    for (auto& face : faces) {
        if (face->check_bounds(hp) && face->same_plane(hp)) {
            return face->normal(hp);
        }
    }
    return {0.0, 0.0, 0.0};
}
