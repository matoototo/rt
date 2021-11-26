#include <iostream>
#include <fstream>

#include "vec3.h"
#include "camera.h"
#include "scene.h"


int main() {
    std::ofstream file("test.ppm");

    Camera cam = Camera(point3(0, 0, 0), 4, 2, 2);
    Image img = Image(1000, 500, cam);
    Scene scene = Scene(1e3);
    scene.add_sphere(std::make_shared<Sphere>(point3{-0.6, 0, -2}, 0.5, color{1, 0, 0}, 0.2));
    scene.add_sphere(std::make_shared<Sphere>(point3{0, -100.5, -2}, 100, color{0, 1, 0}, 0.0));
    scene.add_sphere(std::make_shared<Sphere>(point3{0.6, 0.0, -2}, 0.5, color{1, 1, 1}, 1.2));

    img.fill_pixels(scene.get_fill_func(), 500);
    img.to_ppm(file);

    return 0;
}
