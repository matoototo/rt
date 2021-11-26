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

    Scene() {}

    void add_sphere(const std::shared_ptr<Sphere>&);
    color draw(const int&, const int&, const ray&, const Image&) const;

    draw_func get_fill_func() const {
        return [this](int i, int j, ray& r, Image& img) { return this->draw(i, j, r, img); };
    }

    private:
        std::vector<std::shared_ptr<Sphere>> spheres;
};


void Scene::add_sphere(const std::shared_ptr<Sphere>& sphere) {
    spheres.push_back(sphere);
}

color Scene::draw(const int& i, const int& j, const ray& r, const Image& img) const {
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
        vec3 normal = r.at(t_last) - s_last->center;
        return 0.5*(normal.unit_vec() + 1.0);
    }
    return img.at(i, j);
}
