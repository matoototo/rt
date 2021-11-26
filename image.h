#pragma once

#include <vector>
#include <random>

#include "camera.h"
#include "vec3.h"

double rdbl(int x) {
    if (x == 0) return 0; // lets us have a 'clean' single sample
    static std::normal_distribution<double> norm(0, 0.25);
    static std::uniform_real_distribution<double> unif(0, 1.0);
    static std::default_random_engine re;
    return norm(re);
}

struct Image {
    Image(const int& w, const int& h, const Camera& c): w(w), h(h), c(c) { img.resize(w*h, color(0, 0, 0)); }

    void to_ppm(std::ostream&) const;
    color at(const int& x, const int& y) const { return img[y*w + x]; }
    color& at(const int& x, const int& y) { return img[y*w + x]; }

    constexpr vec3 bottom_left() const {
        return this->c.origin - this->limit_x()/2 - this->limit_y()/2 - vec3(0, 0, this->c.focal_length);
    }
    constexpr vec3 limit_x() const { return vec3(this->c.vpw, 0, 0); }
    constexpr vec3 limit_y() const { return vec3(0, this->c.vph, 0); }

    void fill_pixels(std::function<color (int, int, ray&, Image&)>, int);

    std::vector<color> img;
    const int w, h;
    Camera c;
};

void Image::fill_pixels(std::function<color (int, int, ray&, Image&)> f, int aa_samples = 1) {
    auto bl = this->bottom_left();
    auto lim_x = this->limit_x();
    auto lim_y = this->limit_y();

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            color pixel = {0.0, 0.0, 0.0};
            for (int _ = 0; _ < aa_samples; _++) {
                ray r = ray(this->c.origin, bl + lim_x * (i+1+rdbl(_))/this->w + lim_y * (j+1+rdbl(_))/this->h - this->c.origin);
                pixel = pixel + f(i, j, r, *this);
            }
            this->at(i, j) = pixel / aa_samples;
        }
    }
}

void Image::to_ppm(std::ostream& out) const {
    out << "P3\n" << w << ' ' << h << "\n255\n";
    for (auto j = h-1; j >= 0; j--) {
        for (auto i = 0; i < w; i++) {
            color c = at(i, j) * 255;
            out << int(c.x()) << ' ' << int(c.y()) << ' ' << int(c.z()) << '\n';
        }
        out << '\n';
    }
}