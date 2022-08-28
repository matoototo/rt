#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <limits>
#include <type_traits>

#include "ray.h"
#include "image.h"
#include "mandelbulb.h"
#include "object.h"
#include "sphere.h"
#include "rectangle.h"
#include "cuboid.h"
#include "SSE.h"


using draw_func = std::function<color (int&, int&, ray&, Image&)>;
using gradient = std::vector<color>;
const gradient default_sky = {{201/255., 214/255., 255/255.}, {226/255., 226/255., 226/255.}};

struct Scene {

    Scene(int n_bounces, float fog_factor = 0.0f, gradient sky = default_sky): n_bounces(n_bounces), fog_factor(fog_factor), sky(sky) {}

    void add_sphere(const point3&, const float&, const Props&);
    void add_rectangle(const point3&, const float&, const float&, const face&, const Props&);
    void add_cuboid(const point3&, const float&, const float&, const float&, const Props&);
    void add_mandelbulb(const point3&, const float&, const float&, const float&, const int&, const int&, const int&, const Props&, const vec3&);
    color draw(const int&, const int&, const ray&, const Image&, const int&) const;
    color draw_sky(const int&, const int&, const ray&, const Image&) const;
    bool select_object(const ray&);
    void move_selected_object(const vec3&);
    bool get_hit(const ray&, float&, std::shared_ptr<Object>&) const;

    draw_func get_fill_func() const {
        return [this](int& i, int& j, ray& r, Image& img) { return this->draw(i, j, r, img, n_bounces); };
    }

    private:
        std::vector<std::shared_ptr<Rectangle>> rectangles;
        std::vector<std::shared_ptr<Sphere>> spheres;
        std::vector<std::shared_ptr<Cuboid>> cuboids;
        std::vector<std::shared_ptr<Mandelbulb>> mandelbulbs;
        int n_bounces;
        float fog_factor;
        gradient sky;
        std::shared_ptr<Object> selected = nullptr;

        // SSE acceleration helpers
        #ifdef USE_SSE
        alignas(16) std::vector<float> radii;
        alignas(16) std::vector<float> c0;
        alignas(16) std::vector<float> c1;
        alignas(16) std::vector<float> c2;
        #endif
};


inline void Scene::add_sphere(const point3& center, const float& radius, const Props& props) {
    spheres.push_back(std::make_shared<Sphere>(center, radius, props));

    #ifdef USE_SSE
    radii.push_back(radius);
    c0.push_back(center.e[0]);
    c1.push_back(center.e[1]);
    c2.push_back(center.e[2]);
    #endif
}

inline void Scene::add_rectangle(const point3& o, const float& w, const float& h, const face& f, const Props& props) {
    rectangles.push_back(std::make_shared<Rectangle>(o, w, h, f, props));
}

inline void Scene::add_cuboid(const point3& o, const float& w, const float& h, const float& d, const Props& props) {
    cuboids.push_back(std::make_shared<Cuboid>(o, w, h, d, props));
}

inline void Scene::add_mandelbulb(const point3& o, const float& r, const float& eps, const float& scale, const int& iter, const int& steps, const int& n, const Props& props, const vec3& c) {
    mandelbulbs.push_back(std::make_shared<Mandelbulb>(o, r, eps, scale, iter, steps, n, props, c));
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
    #ifdef USE_NAIVE_SSE
    check_sphere_SSE(objs, r, t_last, o_last);
    #else
    for (auto& x : objs) {
        auto t = x->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = x;
        }
    }
    #endif
}

inline bool Scene::get_hit(const ray& r, float& t_last, std::shared_ptr<Object>& o_last) const {
    check<Rectangle>(this->rectangles, r, t_last, o_last);
    check<Cuboid>(this->cuboids, r, t_last, o_last);
    check<Mandelbulb>(this->mandelbulbs, r, t_last, o_last);

    #ifdef USE_SSE
    smart_check_sphere_SSE(this->spheres, r, t_last, o_last, this->radii, this->c0, this->c1, this->c2);
    #else
    check<Sphere>(this->spheres, r, t_last, o_last);
    #endif

    return o_last != nullptr;
}

inline bool Scene::select_object(const ray& r) {
    float t_last = std::numeric_limits<float>::max();
    std::shared_ptr<Object> o_last = nullptr;
    get_hit(r, t_last, o_last);
    if (selected)
        selected->toggle_select();
    if (selected != o_last and o_last and t_last < 100 and t_last > 0.0001) {
        o_last->toggle_select();
        selected = o_last;
    } else {
        selected = nullptr;
    }

    return o_last != nullptr;
}

inline void Scene::move_selected_object(const vec3& v) {
    if (selected)
        selected->move(v);
}

inline color Scene::draw(const int& i, const int& j, const ray& r, const Image& img, const int& n) const {
    if (n <= 0) return color(0, 0, 0);

    float t_last = std::numeric_limits<float>::max();
    std::shared_ptr<Object> o_last;

    get_hit(r, t_last, o_last);

    point3 hit_point = r.at(t_last);

    ray fog_ray;
    float fog_d;
    const float fog_glow = 0.0;
    const float fog_reflect = 1.0;
    const color fog_color = {1.0, 1.0, 1.0};
    if (fog_factor > 0) {
        // e^{-K*d} = prob. of passing without scatter, K \in [0, 1]
        // => d = -log(prob)/K
        fog_d = -std::log(rand()/(float)RAND_MAX)/fog_factor;
        if (fog_d < std::min((r.orig - hit_point).length(), 5.0f)) {
            fog_ray = ray(r.at(fog_d), vec3::rand());
            return hadamard(fog_reflect*this->draw(i, j, fog_ray, img, n-1), fog_color) + fog_glow*fog_color;
        }
    }

    if (t_last < 100 and t_last > 0.0001) {
        ray new_ray = o_last->scatter(hit_point, r);
        return hadamard(o_last->props.reflect*this->draw(i, j, new_ray, img, n-1), o_last->props.obj_color) + o_last->props.glow*o_last->props.obj_color;
    }
    return draw_sky(i, j, r, img);
}

inline color Scene::draw_sky(const int& i, const int& j, const ray& r, const Image& img) const {
    float mix = r.dir.y()*0.5 + 0.5;
    return (1-mix)*sky[0] + mix*sky[1];
}
