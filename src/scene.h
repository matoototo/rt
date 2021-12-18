#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <limits>
#include <type_traits>

#include "ray.h"
#include "image.h"
#include "object.h"
#include "sphere.h"
#include "rectangle.h"
#include "cuboid.h"


using draw_func = std::function<color (int&, int&, ray&, Image&)>;

struct Scene {

    Scene(int n_bounces): n_bounces(n_bounces) {}

    void add_sphere(const point3&, const float&, const Props&);
    void add_rectangle(const point3&, const float&, const float&, const face&, const Props&);
    void add_cuboid(const point3&, const float&, const float&, const float&, const Props&);
    color draw(const int&, const int&, const ray&, const Image&, const int&) const;
    color draw_sky(const int&, const int&, const ray&, const Image&) const;

    draw_func get_fill_func() const {
        return [this](int& i, int& j, ray& r, Image& img) { return this->draw(i, j, r, img, n_bounces); };
    }

    private:
        std::vector<std::shared_ptr<Rectangle>> rectangles;
        std::vector<std::shared_ptr<Sphere>> spheres;
        std::vector<std::shared_ptr<Cuboid>> cuboids;
        int n_bounces;
};


inline void Scene::add_sphere(const point3& center, const float& radius, const Props& props) {
    spheres.push_back(std::make_shared<Sphere>(center, radius, props));
}

inline void Scene::add_rectangle(const point3& o, const float& w, const float& h, const face& f, const Props& props) {
    rectangles.push_back(std::make_shared<Rectangle>(o, w, h, f, props));
}

inline void Scene::add_cuboid(const point3& o, const float& w, const float& h, const float& d, const Props& props) {
    cuboids.push_back(std::make_shared<Cuboid>(o, w, h, d, props));
}

template <typename T>
inline void check(const std::vector<std::shared_ptr<T>>& objs, const ray& r, float& t_last, std::shared_ptr<Object>& o_last) {
    for (auto& x : objs) {
        auto t = x->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = x;
        }
    }
}

template <> inline void check<Sphere>(const std::vector<std::shared_ptr<Sphere>>& objs, const ray& r, float& t_last, std::shared_ptr<Object>& o_last) {
    for (auto& x : objs) {
        auto t = x->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = x;
        }
    }
}

inline color Scene::draw(const int& i, const int& j, const ray& r, const Image& img, const int& n) const {
    if (n <= 0) return color(0, 0, 0);

    float t_last = std::numeric_limits<float>::max();
    std::shared_ptr<Object> o_last;
    check<Rectangle>(this->rectangles, r, t_last, o_last);
    check<Cuboid>(this->cuboids, r, t_last, o_last);
    check<Sphere>(this->spheres, r, t_last, o_last);

    if (t_last < 100 and t_last > 0.0001) {
        point3 hit_point = r.at(t_last);
        ray new_ray = o_last->scatter(hit_point, r);
        return o_last->props.reflect*this->draw(i, j, new_ray, img, n-1) + std::pow(o_last->props.glow, 1)*o_last->props.obj_color;
    }
    return draw_sky(i, j, r, img);
}

inline color Scene::draw_sky(const int& i, const int& j, const ray& r, const Image& img) const {
    float mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return (1-mix)*color(201/255., 214/255., 255/255.) + mix*color(226/255., 226/255., 226/255.);
}
