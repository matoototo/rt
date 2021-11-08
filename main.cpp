#include <iostream>
#include <fstream>
#include <vector>

#include "vec3.h"
#include "ray.h"
#include "camera.h"

struct Image {
    Image(const int& w, const int& h): w(w), h(h) { img.resize(w*h, color(0, 0, 0)); }

    void to_ppm(std::ostream&) const;
    color at(const int& x, const int& y) const { return img[x*w + y]; }
    color& at(const int& x, const int& y) { return img[x*w + y]; }

    std::vector<color> img;

    private:
        int w, h;
};

void Image::to_ppm(std::ostream& out) const {
    out << "P3\n" << w << ' ' << h << "\n255\n";
    for (auto i = 0; i < h; i++) {
        for (auto j = 0; j < w; j++) {
            color c = at(i, j);
            out << c*255 << '\n';
        }
        out << '\n';
    }
}

Image create_sky(int w, int h, const Camera& c) {
    Image sky = Image(w, h);

    auto limit_x = vec3(c.vpw, 0, 0);
    auto limit_y = vec3(0, c.vph, 0);
    auto bottom_left = c.origin - limit_x/2 - limit_y/2 - vec3(0, 0, c.focal_length);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            ray r = ray(c.origin, limit_x * double(i+1)/w + limit_y * double(j+1)/h);
            double mix = r.dir.unit_vec().y();
            sky.at(i, j) = (1-mix)*color(1.0, 1.0, 1.0) + mix*color(0.5, 0.7, 1.0);
        }
    }
    return sky;
}

int main() {
    std::ofstream file("test.ppm");
    auto img = Image(200, 200);

    auto sky = create_sky(400, 200, Camera(point3(0, 0, 0), 4, 2, 2));
    sky.to_ppm(file);

    return 0;
}
