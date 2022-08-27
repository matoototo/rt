#include <iostream>
#include <fstream>
#include <thread>

#include "vec3.h"
#include "camera.h"
#include "config.h"
#include "props.h"
#include "scene.h"
#include "rectangle.h"
#include "util.h"

#ifdef INTERACTIVE
#include "window.h"

sf::Image Image_to_SFML(Image image) {
    auto pixels = image.img;
    auto w = image.w;
    auto h = image.h;
    sf::Image sfml_img;
    sfml_img.create(w, h, sf::Color::Black);
    for (int j = 1; j < h; ++j) {
        for (int i = 0; i < w; i++) {
            color c = pixels[j*w + i];
            sfml_img.setPixel(i, h-j, sf::Color(int(c.x() * 255), int(c.y() * 255), int(c.z() * 255)));
        }
    }
    return sfml_img;
}
#endif

Image& compute_next(Image& img, Config& conf, const Scene& scene, bool first = false) {
    int samples = conf.json["samples"];
    int n_threads = conf.json["n_threads"];

    #ifdef PRECOMPUTE_RDBL
    // precompute floats
    seedrdbl();
    #endif

    std::vector<std::thread> threads;
    for (auto i = 0; i < n_threads; ++i) {
        threads.emplace_back(([&, i]() { img.fill_pixels(scene.get_fill_func(), samples, i, n_threads); }));
    }

    for (auto i = 0; i < n_threads; ++i) {
        threads[i].join();
    }

    if (!first)
        img *= 0.5;

    conf.json["samples"] = samples * 2;

    std::ofstream file("test.ppm");
    img.to_ppm(file);
    return img;
}


int main() {
    std::ifstream json_file("config.json");
    auto conf = Config(json_file);

    gradient sky_gradient = {json_to_vec3(conf.json["sky_gradient"][0]),
                             json_to_vec3(conf.json["sky_gradient"][1])};

    Scene scene = Scene(conf.json["max_bounces"], conf.json["fog_factor"], sky_gradient);
    Camera cam = Camera(json_to_vec3(conf.json["cam_origin"]), json_to_vec3(conf.json["cam_at"]),
                                     conf.json["aspect_ratio"], conf.json["fov"]);

    conf.add_objects(scene);

    Image img = Image(conf.json["width"], conf.json["height"], cam);

    compute_next(img, conf, scene, true); // TODO: figure out how to purify

    #ifdef INTERACTIVE

    auto sfimg = Image_to_SFML(img);
    Window window(conf.json["width"], conf.json["height"], [&]() { return Image_to_SFML(compute_next(img, conf, scene)); }, "rt");
    window.update(sfimg);
    window.show();

    #endif

    return 0;
}
