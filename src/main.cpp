#include <iostream>
#include <fstream>

#include "vec3.h"
#include "camera.h"
#include "props.h"
#include "scene.h"
#include "rectangle.h"


int main() {
    std::ofstream file("test.ppm");

    Camera cam = Camera(point3(0, 0, 0), 4, 2, 2);
    Image img = Image(1000, 500, cam);
    Scene scene = Scene(1e5);

    Props matte = Props(0.0, {0.0, 0.0, 0.0}, 0.2);
    Props light = matte.glowed(2.5);
    Props mirror = Props(0.8, {0.0, 0.0, 0.0}, 0.15, 0.5);

    scene.add_sphere(point3{-0.6, 0, -2}, 0.5, mirror.colored({1, 0, 0}));
    scene.add_sphere(point3{0, -100.5, -2}, 100, matte);
    scene.add_sphere(point3{0.6, 0, -2}, 0.5, light.colored({1, 1, 1}));

    scene.add_rectangle(point3{-2.0, -0.7, -2.75}, 4, 1.5, z_side, matte);
    scene.add_rectangle(point3{-2.0, -0.7, -2.75}, -4, 1.5, x_side, matte);
    scene.add_rectangle(point3{2.0, -0.7, -2.75}, -4, 1.5, x_side, matte);

    scene.add_cuboid(point3{0.75, -0.6, -1.5}, 0.25, 0.75, 0.25, matte.colored({0, 1, 0}));
    scene.add_cuboid(point3{-0.175, 0.5, -2.25}, 0.35, 0.35, 0.35, matte.colored({0, 0, 1.0}));

    img.fill_pixels(scene.get_fill_func(), 500);
    img.to_ppm(file);

    return 0;
}
