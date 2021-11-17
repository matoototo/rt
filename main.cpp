#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "sphere.h"

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

    void fill_pixels(std::function<color (int, int, Image&)>);

    std::vector<color> img;
    const int w, h;
    Camera c;
};

void Image::fill_pixels(std::function<color (int, int, Image&)> f) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            this->at(i, j) = f(i, j, *this);
        }
    }
}

void Image::to_ppm(std::ostream& out) const {
    out << "P3\n" << w << ' ' << h << "\n255\n";
    for (auto j = 0; j < h; j++) {
        for (auto i = 0; i < w; i++) {
            color c = at(i, j) * 255;
            out << int(c.x()) << ' ' << int(c.y()) << ' ' << int(c.z()) << '\n';
        }
        out << '\n';
    }
}

color create_sky(int i, int j, const Image& img) {
    auto bl = img.bottom_left();
    auto lim_x = img.limit_x();
    auto lim_y = img.limit_y();
    ray r = ray(img.c.origin, bl + lim_x * double(i+1)/img.w + lim_y * double(j+1)/img.h - img.c.origin);
    double mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return mix*color(1.0, 1.0, 1.0) + (1-mix)*color(0.5, 0.7, 1.0);
}

int main() {
    std::ofstream file("test.ppm");

    Camera cam = Camera(point3(0, 0, 0), 4, 2, 2);
    Image sky = Image(200, 100, cam);

    sky.fill_pixels(create_sky);
    sky.to_ppm(file);

    return 0;
}
