#pragma once

#include "object.h"
#include "props.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

struct Mandelbulb : Object {
    Mandelbulb(const point3& o, const float& r, const float& eps, const int& n_iter, const int& steps, const int& n, const Props& props,
               const vec3& c = {0, 0, 0}): Object(props), o(o), boundingbox(o, r, props), eps(eps), n_iter(n_iter), steps(steps), n(n), c(c) {}

    void iterate(point3& p, float& dw) const;
    float distance(const point3& current_point) const;

    float DE(vec3 pos) const;

    float hit(const ray& r) const;
    vec3 normal(const point3& hp, const ray& r) const;

    float eps;
    int steps, n, n_iter;
    vec3 c, o;
    Sphere boundingbox;
};

inline float Mandelbulb::distance(const point3& current_point) const {
    // White-Nylander formula, I did not come up with this...
    vec3 z = current_point;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < n_iter; i++) {
		r = z.length();
		if (r > 10) break;
		float zr = pow(r, n);
		dr = pow(r, n-1.0)*n*dr + 1.0;

		float theta = n*acos(z.z()/r);
		float phi = n*atan2(z.y(), z.x());
		z = zr * point3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z = z + current_point;
	}
	return 0.5*log(r)*r/dr;
}

inline float Mandelbulb::hit(const ray& r) const {
    point3 current_point = r.orig - this->o;
    if (distance(current_point) < 4*eps) {
        current_point = r.at(0.01) - this->o; // escape surface if already on it
    } else {
        float bounding_t = boundingbox.hit(r);
        if (dblequ(bounding_t, -1)) return -1;
        current_point = r.at(bounding_t*0.95) - this->o;
    }

    float dist;
    float dist_sum = 0.0f, okay_sum = 0.0f;
    for (auto i = 0; i < steps; ++i) {
        dist = distance(current_point);
        dist_sum += 0.99*dist;
        if (dist < eps) break;
        else if (dist < 4*eps) okay_sum = dist_sum; // save close enough, if we don't get any closer
        current_point = r.orig + r.dir * dist_sum - this->o;
    }
    if (dist < eps) return dist_sum;
    else if (!dblequ(okay_sum, 0.0f)) return okay_sum;
    else return -1.f;
}

inline vec3 Mandelbulb::normal(const point3& hp, const ray& r) const {

    point3 hp_back = hp - eps * r.dir;

    point3 x1 = hp_back - point3(eps, 0.f, 0.f);
    point3 x2 = hp_back + point3(eps, 0.f, 0.f);
    point3 y1 = hp_back - point3(0.f, eps, 0.f);
    point3 y2 = hp_back + point3(0.f, eps, 0.f);
    point3 z1 = hp_back - point3(0.f, 0.f, eps);
    point3 z2 = hp_back + point3(0.f, 0.f, eps);

    vec3 surf_normal = vec3(distance(x1)-distance(x2),
                            distance(y1)-distance(y2),
                            distance(z1)-distance(z2));
    return surf_normal.unit_vec();
}
