#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <limits>

#include "ray.h"
#include "image.h"
#include "sphere.h"


using draw_func = std::function<color (int, int, ray&, Image&)>;

struct Scene {

    Scene(int n_bounces): n_bounces(n_bounces) {}

    void add_sphere(const std::shared_ptr<Sphere>&);
    color draw(const int&, const int&, const ray&, const Image&, const int&) const;
    color draw_sky(const int&, const int&, const ray&, const Image&) const;

    draw_func get_fill_func() const {
        return [this](int i, int j, ray& r, Image& img) { return this->draw(i, j, r, img, n_bounces); };
    }

    private:
        std::vector<std::shared_ptr<Sphere>> spheres;
        int n_bounces;
};


inline void Scene::add_sphere(const std::shared_ptr<Sphere>& sphere) {
    spheres.push_back(sphere);
}

inline color Scene::draw(const int& i, const int& j, const ray& r, const Image& img, const int& n) const {
    if (n <= 0) return color(0, 0, 0);

    double t_last = std::numeric_limits<double>::max();
    std::shared_ptr<Sphere> s_last;
    for (auto& x : spheres) {
        auto t = x->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            s_last = x;
        }
    }

    if (t_last < std::numeric_limits<double>::max()) {
        point3 hit_point = r.at(t_last);
        vec3 normal = hit_point - s_last->center;
        ray new_ray = ray(hit_point, normal.unit_vec() + vec3::rand().unit_vec());
        return 0.5*this->draw(i, j, new_ray, img, n-1) + std::pow(s_last->glow, n_bounces-n+1)*s_last->color;
    }
    return draw_sky(i, j, r, img);
}

inline color Scene::draw_sky(const int& i, const int& j, const ray& r, const Image& img) const {
    double mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return (1-mix)*color(1.0, 1.0, 1.0) + mix*color(0.5, 0.7, 1.0);
}
