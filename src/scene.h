#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <limits>

#include "ray.h"
#include "image.h"
#include "object.h"
#include "sphere.h"
#include "rectangle.h"
#include "cuboid.h"


using draw_func = std::function<color (int, int, ray&, Image&)>;

struct Scene {

    Scene(int n_bounces): n_bounces(n_bounces) {}

    void add_sphere(const point3&, const double&, const color&, const double&);
    void add_rectangle(const point3&, const double&, const double&, const color&, const double&, const face&);
    void add_cuboid(const point3&, const double&, const double&, const double&, const color&, const double&);
    color draw(const int&, const int&, const ray&, const Image&, const int&) const;
    color draw_sky(const int&, const int&, const ray&, const Image&) const;

    draw_func get_fill_func() const {
        return [this](int i, int j, ray& r, Image& img) { return this->draw(i, j, r, img, n_bounces); };
    }

    private:
        std::vector<std::shared_ptr<Object>> objects;
        int n_bounces;
};


inline void Scene::add_sphere(const point3& center, const double& radius, const color& color, const double& gl) {
    objects.push_back(std::make_shared<Sphere>(center, radius, color, gl));
}

inline void Scene::add_rectangle(const point3& o, const double& w, const double& h, const color& color, const double& gl, const face& f) {
    objects.push_back(std::make_shared<Rectangle>(o, w, h, color, gl, f));
}

inline void Scene::add_cuboid(const point3& o, const double& w, const double& h, const double& d, const color& color, const double& gl) {
    objects.push_back(std::make_shared<Cuboid>(o, w, h, d, color, gl));
}

inline color Scene::draw(const int& i, const int& j, const ray& r, const Image& img, const int& n) const {
    if (n <= 0) return color(0, 0, 0);

    double t_last = std::numeric_limits<double>::max();
    std::shared_ptr<Object> o_last;
    for (auto& x : objects) {
        auto t = x->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = x;
        }
    }

    if (t_last < std::numeric_limits<double>::max()) {
        point3 hit_point = r.at(t_last);
        ray new_ray = o_last->scatter(hit_point, r);
        return 0.5*this->draw(i, j, new_ray, img, n-1) + std::pow(o_last->glow, 1)*o_last->obj_color;
    }
    return draw_sky(i, j, r, img);
}

inline color Scene::draw_sky(const int& i, const int& j, const ray& r, const Image& img) const {
    double mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return (1-mix)*color(1.0, 1.0, 1.0) + mix*color(0.5, 0.7, 1.0);
}
