#pragma once

#include <random>
#include <vector>

std::vector<double> rdbl_vec;

inline double rdbl(int x = 1) {
    if (x == 0) return 0; // lets us have a 'clean' single sample
    #ifdef PRECOMPUTE_RDBL
    thread_local static int c = 0;
    return rdbl_vec[++c % rdbl_vec.size()];
    #else
    thread_local static std::normal_distribution<double> norm(0, 0.25);
    thread_local static std::minstd_rand re;
    return norm(re);
    #endif
}

inline void seedrdbl() {
    std::normal_distribution<double> norm(0, 0.25);
    std::minstd_rand re;
    rdbl_vec.resize(100000);
    rdbl_vec[0] = 0.;
    for (auto i = 1; i < rdbl_vec.size(); ++i) {
        rdbl_vec[i] = norm(re);
    }
}


inline bool dblequ(const double& x, const double& y, const double& EPS = 0.0001) {
    return std::abs(x-y) < EPS;
}
