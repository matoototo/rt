#pragma once

#include <limits>

#include "object.h"
#include "ray.h"

enum face { x_side, y_side, z_side };

struct Rectangle : Object {
    Rectangle(const point3& o, const double& w, const double& h, const color& color, const double& gl, face face):
                    Object(gl, color), orig(o), w(w), h(h), face(face) {}

    double hit(const ray& r) const;
    vec3 normal(const point3& hp) const;
    inline bool check_bounds(const point3& hp) const;
    inline bool same_plane(const point3& hp) const;

    point3 orig;
    double w, h;
    face face;
};

inline bool is_between(double x, double a, double b) {
    if (a > b) std::swap(a, b);
    return x >= a && x <= b;
}

inline bool Rectangle::check_bounds(const point3& hp) const {
    switch (face) {
        case x_side: return is_between(hp.z(), orig.z(), orig.z() - w) && is_between(hp.y(), orig.y(), orig.y() + h);
        case y_side: return is_between(hp.x(), orig.x(), orig.x() + w) && is_between(hp.z(), orig.z(), orig.z() - h);
        case z_side: return is_between(hp.x(), orig.x(), orig.x() + w) && is_between(hp.y(), orig.y(), orig.y() + h);
    }
}

inline bool Rectangle::same_plane(const point3& hp) const {
    switch (face) {
        case x_side: return dblequ(orig.x(), hp.x());
        case y_side: return dblequ(orig.y(), hp.y());
        case z_side: return dblequ(orig.z(), hp.z());
    }
}


inline double Rectangle::hit(const ray& r) const {
    if (face == x_side && r.dir.x() == 0.) return -1;
    if (face == y_side && r.dir.y() == 0.) return -1;
    if (face == z_side && r.dir.z() == 0.) return -1;

    double t;
    if (face == x_side) t = (orig.x() - r.orig.x())/r.dir.x();
    else if (face == y_side) t = (orig.y() - r.orig.y())/r.dir.y();
    else t = (orig.z() - r.orig.z())/r.dir.z();

    point3 hp = r.at(t); // hit point
    if (check_bounds(hp)) {
        return t;
    }
    return -1;
}

inline vec3 Rectangle::normal(const point3& hp) const {
    switch (face) {
        case x_side: return vec3(1.0, 0.0, 0.0);
        case y_side: return vec3(0.0, 1.0, 0.0);
        case z_side: return vec3(0.0, 0.0, 1.0);
    }
};