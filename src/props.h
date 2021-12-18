#pragma once

#include "vec3.h"

struct Props {
    Props(const float& smooth = 0.0, const color& obj_color = {0, 0, 0}, const float& glow = 0, const float& reflect = 0.5):
          smooth(smooth), obj_color(obj_color), glow(glow), reflect(reflect) {}

    float reflect;
    color obj_color;
    float glow;
    float smooth;

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
