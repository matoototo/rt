#include <iostream>
#include <fstream>

#include "vec3.h"
#include "camera.h"
#include "scene.h"

color create_sky(const int& i, const int& j, const ray& r, const Image& img) {
    double mix = r.dir.unit_vec().y()*0.5 + 0.5;
    return (1-mix)*color(1.0, 1.0, 1.0) + mix*color(0.5, 0.7, 1.0);
}

int main() {
    std::ofstream file("test.ppm");

    Camera cam = Camera(point3(0, 0, 0), 4, 2, 2);
    Image img = Image(400, 200, cam);
    Scene scene = Scene();
    scene.add_sphere(std::make_shared<Sphere>(point3{0, 0, -2}, 0.5, color{1, 0, 0}));
    scene.add_sphere(std::make_shared<Sphere>(point3{0, -100.5, -2}, 100, color{1, 0, 0}));
    scene.add_sphere(std::make_shared<Sphere>(point3{0, 0, -4}, 1.5, color{1, 0, 0}));


    img.fill_pixels(create_sky);
    img.fill_pixels(scene.get_fill_func(), 1);
    img.to_ppm(file);

    return 0;
}
