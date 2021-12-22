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


int main() {
    std::ifstream json_file("config.json");
    auto conf = Config(json_file);

    gradient sky_gradient = {json_to_vec3(conf.json["sky_gradient"][0]),
                             json_to_vec3(conf.json["sky_gradient"][1])};

    Scene scene = Scene(conf.json["max_bounces"], conf.json["fog_factor"], sky_gradient);
    Camera cam = Camera(json_to_vec3(conf.json["cam_origin"]), conf.json["vpw"],
                                     conf.json["vph"], conf.json["focal_length"]);
    Image img = Image(conf.json["width"], conf.json["height"], cam);

    int samples = conf.json["samples"];
    int n_threads = conf.json["n_threads"];

    conf.add_objects(scene);

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

    std::ofstream file("test.ppm");
    img.to_ppm(file);

    return 0;
}
