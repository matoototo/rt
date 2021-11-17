#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include "vec3.h"
#include "ray.h"
#include "camera.h"

struct Image {
    Image(const int& w, const int& h): w(w), h(h) { img.resize(w*h, color(0, 0, 0)); }

    void to_ppm(std::ostream&) const;
    color at(const int& x, const int& y) const { return img[y*w + x]; }
    color& at(const int& x, const int& y) { return img[y*w + x]; }

    void fill_pixels(std::function<color (int, int, Image&)>);

    std::vector<color> img;
    int w, h;
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

color create_sky(int i, int j, const Image& img, const Camera& c) {
    auto limit_x = vec3(c.vpw, 0, 0);
    auto limit_y = vec3(0, c.vph, 0);
    auto bottom_left = c.origin - limit_x/2 - limit_y/2 - vec3(0, 0, c.focal_length);

    ray r = ray(c.origin, bottom_left + limit_x * double(i+1)/img.w + limit_y * double(j+1)/img.h - c.origin);
    double mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return mix*color(1.0, 1.0, 1.0) + (1-mix)*color(0.5, 0.7, 1.0);
}

int main() {
    std::ofstream file("test.ppm");

    Image sky = Image(200, 100);
    sky.fill_pixels([sky](int i, int j, const Image& img) { return create_sky(i, j, img, Camera(point3(0, 0, 0), 4, 2, 2)); });
    sky.to_ppm(file);

    return 0;
}
