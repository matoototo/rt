#pragma once

#include "vec3.h"

struct Props {
    Props(const double& smooth = 0.0, const color& obj_color = {0, 0, 0}, const double& glow = 0, const double& reflect = 0.5):
          smooth(smooth), obj_color(obj_color), glow(glow), reflect(reflect) {}

    double reflect;
    color obj_color;
    double glow;
    double smooth;

    Props colored(const color& c) const {
        return Props(smooth, c, glow, reflect);
    }

    Props glowed(const double& g) const {
        return Props(smooth, obj_color, g, reflect);
    }

    Props reflecting(const double& r) const {
        return Props(smooth, obj_color, glow, r);
    }

    Props smoothing(const double& s) const {
        return Props(s, obj_color, glow, reflect);
    }
};
