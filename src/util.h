#pragma once

#include <random>

inline double rdbl(int x = 1) {
    if (x == 0) return 0; // lets us have a 'clean' single sample
    thread_local static std::normal_distribution<double> norm(0, 0.25);
    thread_local static std::minstd_rand re;
    return norm(re);
}

inline bool dblequ(const double& x, const double& y, const double& EPS = 0.0001) {
    return std::abs(x-y) < EPS;
}
