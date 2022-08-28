#pragma once

#include <string>

#include "props.h"
#include "ray.h"

struct Object {
    Object(point3 o, Props props): orig(o), props(props), orig_props(props), name(props.name) {}

    virtual float hit(const ray& r) const = 0;
    virtual vec3 normal(const point3& hp, const ray& r) const = 0;
    bool toggle_select();
    virtual void move(const vec3&);
    ray scatter(const point3& hp, const ray& r_in) const;

    bool selected = false;
    point3 orig;
    Props props;
    Props orig_props;
    std::string name;
};

inline ray Object::scatter(const point3& hp, const ray& r_in) const {
    vec3 normal = this->normal(hp, r_in);
    if (r_in.dir * normal > 0) normal = -normal;

    vec3 dir = {0.0, 0.0, 0.0};
    vec3 h = -2*(r_in.dir * normal)*normal;

    dir = dir + props.smooth*(r_in.dir+h).unit_vec();
    dir = dir + (1-props.smooth)*(normal + vec3::rand().unit_vec()).unit_vec();

    return ray(hp, dir);
}

inline bool Object::toggle_select() {
    selected = !selected;
    if (!selected) {
        props = orig_props;
    } else {
        props = SELECTED;
    }
    return selected;
}

inline void Object::move(const vec3& v) {
    orig = orig + v;
}
