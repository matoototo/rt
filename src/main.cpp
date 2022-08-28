#include <iostream>
#include <fstream>
#include <thread>

#include "vec3.h"
#include "camera.h"
#include "config.h"
#include "scene.h"
#include "rectangle.h"
#include "util.h"
#include "ray.h"

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
            color c = pixels[j*w + i].clamp(0, 1);
            sfml_img.setPixel(i, h-j, sf::Color(int(c.x() * 255), int(c.y() * 255), int(c.z() * 255)));
        }
    }
    return sfml_img;
}
#endif

Image& compute_next(Image& img, Config& conf, const Scene& scene) {
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

    if (conf.json["samples"] != conf.json["samples_orig"])
        img *= 0.5;

    conf.json["samples"] = samples * 2;

    std::ofstream file("test.ppm");
    img.to_ppm(file);
    return img;
}

void init_img(Image& img, std::shared_ptr<Camera> cam, Config& conf) {
    conf.json["samples"] = conf.json["samples_orig"];
    img = Image(conf.json["width"], conf.json["height"], cam);
}


int main() {
    std::ifstream json_file("config.json");
    auto conf = Config(json_file);
    conf.json["samples_orig"] = conf.json["samples"];

    gradient sky_gradient = {json_to_vec3(conf.json["sky_gradient"][0]),
                             json_to_vec3(conf.json["sky_gradient"][1])};

    Scene scene = Scene(conf.json["max_bounces"], conf.json["fog_factor"], sky_gradient);
    std::shared_ptr<Camera> cam = std::make_shared<Camera>(json_to_vec3(conf.json["cam_origin"]), json_to_vec3(conf.json["cam_at"]),
                                     conf.json["aspect_ratio"], conf.json["fov"]);

    conf.add_objects(scene);

    Image img = Image(conf.json["width"], conf.json["height"], cam);

    compute_next(img, conf, scene); // TODO: figure out how to purify

    #ifdef INTERACTIVE

    sf::Image sfimg = Image_to_SFML(img);
    auto next_func = [&]() { return Image_to_SFML(compute_next(img, conf, scene)); };
    auto select_func = [&](int x, int y) {
        init_img(img, cam, conf);
        return scene.select_object(cam->get_ray(x/double(img.w), y/double(img.h)));
    };
    auto cam_move_func = [&](const vec3& dir) {
        cam->move(conf.json["cam_step"] * dir);
        init_img(img, cam, conf);
    };
    auto move_obj_func = [&](const vec3& dir) {
        scene.move_selected_object(conf.json["move_step"] * dir);
        init_img(img, cam, conf);
    };

    Window window(conf.json["width"], conf.json["height"], next_func, select_func, cam_move_func, move_obj_func, "rt");
    window.update(sfimg);
    window.show();

    #endif

    return 0;
}
