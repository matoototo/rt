#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "props.h"
#include "scene.h"
#include "vec3.h"

vec3 json_to_vec3(const nlohmann::json&);

struct Config {
    Config(std::ifstream& file) {
        file >> json;
    }

    void add_objects(Scene& scene) {
        for (auto& o : json["objects"]) {
            vec3 pos = json_to_vec3(o["pos"]);
            Props props = create_props(o["props"]);
            if (o["type"] == "sphere") {
                scene.add_sphere(pos, o["radius"], props);
            } else if (o["type"] == "rectangle") {
                scene.add_rectangle(pos, o["w"], o["h"], o["face"], props);
            } else if (o["type"] == "cuboid") {
                scene.add_cuboid(pos, o["w"], o["h"], o["d"], props);
            } else if (o["type"] == "mandelbulb") {
                scene.add_mandelbulb(pos, o["r"], o["eps"], o["scale"], o["iter"], o["steps"], o["n"], props, json_to_vec3(o["c"]));
            }
        }
    }

    Props create_props(const nlohmann::json& o) {
        color c = json_to_vec3(o["color"]);
        std::string name = o.contains("name") ? o["name"].get<std::string>() : "";
        return Props(o["smooth"], c, o["glow"], o["reflect"], name);
    }

    nlohmann::json json;
};

inline vec3 json_to_vec3(const nlohmann::json& j) {
    return vec3(j[0], j[1], j[2]);
}
