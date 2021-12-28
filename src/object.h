#pragma once

#include "props.h"
#include "ray.h"

struct Object {
    Object(Props props): props(props) {}

    virtual float hit(const ray& r) const = 0;
    virtual vec3 normal(const point3& hp) const = 0;
    ray scatter(const point3& hp, const ray& r_in) const;

    Props props;
};

inline ray Object::scatter(const point3& hp, const ray& r_in) const {
    vec3 normal = this->normal(hp);
    if (r_in.dir * normal > 0) normal = -normal;

    vec3 dir = {0.0, 0.0, 0.0};
    vec3 h = -2*(r_in.dir * normal)*normal;

    dir = dir + props.smooth*(r_in.dir+h).unit_vec();
    dir = dir + (1-props.smooth)*(normal + vec3::rand().unit_vec()).unit_vec();

    return ray(hp, dir);
}
