#include <iostream>
#include <fstream>

#include "vec3.h"
#include "camera.h"
#include "scene.h"
#include "rectangle.h"


int main() {
    std::ofstream file("test.ppm");

    Camera cam = Camera(point3(0, 0, 0), 4, 2, 2);
    Image img = Image(1000, 500, cam);
    Scene scene = Scene(1e3);
    scene.add_sphere(point3{-0.6, 0, -2}, 0.5, color{1, 0, 0}, 0.2);
    scene.add_sphere(point3{0, -100.5, -2}, 100, color{0, 1, 0}, 0.0);
    scene.add_sphere(point3{0.6, 0, -2}, 0.5, color{1, 1, 1}, 1.9);
    scene.add_rectangle(point3{-2.0, -0.7, -2.75}, 4, 1.5, color{0, 0, 0}, 1.1, z_side);
    scene.add_rectangle(point3{-2.0, -0.7, -2.75}, -4, 1.5, color{0, 0, 0}, 1.1, x_side);
    scene.add_rectangle(point3{2.0, -0.7, -2.75}, -4, 1.5, color{0, 0, 0}, 1.1, x_side);

    img.fill_pixels(scene.get_fill_func(), 500);
    img.to_ppm(file);

    return 0;
}
