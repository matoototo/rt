#pragma once

#include <vector>
#include <functional>

#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "util.h"


struct Image {
    Image(const int& w, const int& h, const Camera& c): w(w), h(h), c(c) { img.resize(w*h, color(0, 0, 0)); }

    void to_ppm(std::ostream&) const;
    color at(const int& x, const int& y) const { return img[y*w + x]; }
    color& at(const int& x, const int& y) { return img[y*w + x]; }

    void fill_pixels(const std::function<color (int&, int&, ray&, Image&)>&, int, int, int);

    std::vector<color> img;
    const int w, h;
    Camera c;
};

inline void Image::fill_pixels(const std::function<color (int&, int&, ray&, Image&)>& f, int aa_samples = 1, int thread_id = 0, int n_threads = 1) {
    for (int j = 0; j < h; ++j) {
        for (int i = 0 + thread_id; i < w; i += n_threads) {
            color pixel = {0.0, 0.0, 0.0};
            for (int _ = 0; _ < aa_samples; _++) {
                ray r = c.get_ray((i+1+rdbl(_))/w, (j+1+rdbl(_))/h);
                pixel = pixel + f(i, j, r, *this);
            }
            this->at(i, j) = (pixel / aa_samples);
        }
    }
}

inline void Image::to_ppm(std::ostream& out) const {
    out << "P3\n" << w << ' ' << h << "\n255\n";
    for (auto j = h-1; j >= 0; j--) {
        for (auto i = 0; i < w; i++) {
            color c = at(i, j) * 255;
            out << int(c.x()) << ' ' << int(c.y()) << ' ' << int(c.z()) << '\n';
        }
        out << '\n';
    }
}