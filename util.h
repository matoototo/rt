#pragma once

#include <random>

inline double rdbl(int x = 1) {
    if (x == 0) return 0; // lets us have a 'clean' single sample
    static std::normal_distribution<double> norm(0, 0.25);
    static std::default_random_engine re;
    return norm(re);
}
