#pragma once

#include <string>

#include "vec3.h"

struct Props {
    Props(const float& smooth = 0.0, const color& obj_color = {0, 0, 0}, const float& glow = 0, const float& reflect = 0.5, const std::string& name = "unnamed"):
          reflect(reflect), obj_color(obj_color), glow(glow), smooth(smooth), name(name) {}

    float reflect;
    color obj_color;
    float glow;
    float smooth;
    std::string name;

    Props colored(const color& c) const {
        return Props(smooth, c, glow, reflect);
    }

    Props glowed(const float& g) const {
        return Props(smooth, obj_color, g, reflect);
    }

    Props reflecting(const float& r) const {
        return Props(smooth, obj_color, glow, r);
    }

    Props smoothing(const float& s) const {
        return Props(s, obj_color, glow, reflect);
    }
};

inline Props SELECTED = Props(0.0, {1, 1, 1}, 0.9, 0.5);
