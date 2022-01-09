#pragma once

#include <vector>
#include <memory>
#include <immintrin.h>
#include <xmmintrin.h>
#include "object.h"
#include "ray.h"
#include "sphere.h"

__m128 inline mul_vec3(const __m128& a_x, const __m128& a_y, const __m128& a_z, const __m128& b_x, const __m128& b_y, const __m128& b_z) {
    __m128 ret = _mm_add_ps(_mm_mul_ps(a_x, b_x), _mm_mul_ps(a_y, b_y));
    return _mm_add_ps(_mm_mul_ps(a_z, b_z), ret);
}

__m128 inline vec3_sq(const __m128& a_x, const __m128& a_y, const __m128& a_z) {
    __m128 ret = _mm_add_ps(_mm_mul_ps(a_x, a_x), _mm_mul_ps(a_y, a_y));
    return _mm_add_ps(_mm_mul_ps(a_z, a_z), ret);
}

void inline check_sphere_SSE(const std::vector<std::shared_ptr<Sphere>>& spheres, const ray& r, float& t_last, std::shared_ptr<Object>& o_last) {

    __m128 ray_ox = _mm_load_ps1(&r.orig.e[0]);
    __m128 ray_oy = _mm_load_ps1(&r.orig.e[1]);
    __m128 ray_oz = _mm_load_ps1(&r.orig.e[2]);

    __m128 ray_dx = _mm_load_ps1(&r.dir.e[0]);
    __m128 ray_dy = _mm_load_ps1(&r.dir.e[1]);
    __m128 ray_dz = _mm_load_ps1(&r.dir.e[2]);

    __m128 a = _mm_set_ps1(r.dir * r.dir);
    __m128 inv2a = _mm_set_ps1(1.f / (-2 * r.dir * r.dir));

    const __m128 two_const = _mm_set_ps1(2.0f);
    const __m128 neg_two_const = _mm_set_ps1(-2.0f);
    const __m128 four_const = _mm_set_ps1(4.0f);

    if (spheres.size() > 3) {
        for (auto i = 3; i < spheres.size(); i += 4) {
            __m128 sph_r = _mm_set_ps(spheres[i]->radius, spheres[i-1]->radius, spheres[i-2]->radius, spheres[i-3]->radius);

            __m128 oc_x = _mm_set_ps(spheres[i]->center.e[0], spheres[i-1]->center.e[0], spheres[i-2]->center.e[0], spheres[i-3]->center.e[0]);
                   oc_x = _mm_sub_ps(ray_ox, oc_x);
            __m128 oc_y = _mm_set_ps(spheres[i]->center.e[1], spheres[i-1]->center.e[1], spheres[i-2]->center.e[1], spheres[i-3]->center.e[1]);
                   oc_y = _mm_sub_ps(ray_oy, oc_y);
            __m128 oc_z = _mm_set_ps(spheres[i]->center.e[2], spheres[i-1]->center.e[2], spheres[i-2]->center.e[2], spheres[i-3]->center.e[2]);
                   oc_z = _mm_sub_ps(ray_oz, oc_z);

            __m128 b = _mm_mul_ps(mul_vec3(oc_x, oc_y, oc_z, ray_dx, ray_dy, ray_dz), two_const);
            __m128 c = _mm_sub_ps(vec3_sq(oc_x, oc_y, oc_z), _mm_mul_ps(sph_r, sph_r));

            __m128 disc = _mm_sub_ps(_mm_mul_ps(b, b), _mm_mul_ps(_mm_mul_ps(a, c), four_const));
            __m128 sqrt_disc = _mm_sqrt_ps(disc);

            __m128 t = _mm_mul_ps(_mm_add_ps(sqrt_disc, b), inv2a); // (-b - sqrt(disc))/(2*a)

            for (auto j = 0; j < 4; ++j) {
                if (t[j] > 0 && t[j] < t_last) {
                    t_last = t[j];
                    o_last = spheres[i - 3 + j];
                }
            }
        }
    }

    // compute spheres on tail the standard way (maximum 3)
    for (auto i = spheres.size() - spheres.size()%4; i < spheres.size(); ++i) {
        auto t = spheres[i]->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = spheres[i];
        }
    }
}

void inline smart_check_sphere_SSE(const std::vector<std::shared_ptr<Sphere>>& spheres, const ray& r, float& t_last, std::shared_ptr<Object>& o_last,
                                   const std::vector<float>& radii, const std::vector<float>& c0, const std::vector<float>& c1, const std::vector<float>& c2) {

    __m128 ray_ox = _mm_load_ps1(&r.orig.e[0]);
    __m128 ray_oy = _mm_load_ps1(&r.orig.e[1]);
    __m128 ray_oz = _mm_load_ps1(&r.orig.e[2]);

    __m128 ray_dx = _mm_load_ps1(&r.dir.e[0]);
    __m128 ray_dy = _mm_load_ps1(&r.dir.e[1]);
    __m128 ray_dz = _mm_load_ps1(&r.dir.e[2]);

    __m128 a = vec3_sq(ray_dx, ray_dy, ray_dz);
    __m128 inv2a = _mm_set_ps1(1.f / (-2 * r.dir * r.dir));

    const __m128 two_const = _mm_set_ps1(2.0f);
    const __m128 neg_two_const = _mm_set_ps1(-2.0f);
    const __m128 four_const = _mm_set_ps1(4.0f);

    if (spheres.size() > 3) {
        for (auto i = 0; i < spheres.size()-3; i += 4) {
            __m128 sph_r = _mm_load_ps(radii.data() + i);
            __m128 oc_x = _mm_load_ps(c0.data() + i);
                   oc_x = _mm_sub_ps(ray_ox, oc_x);
            __m128 oc_y = _mm_load_ps(c1.data() + i);
                   oc_y = _mm_sub_ps(ray_oy, oc_y);
            __m128 oc_z = _mm_load_ps(c2.data() + i);
                   oc_z = _mm_sub_ps(ray_oz, oc_z);

            __m128 b = _mm_mul_ps(mul_vec3(oc_x, oc_y, oc_z, ray_dx, ray_dy, ray_dz), two_const);
            __m128 c = _mm_sub_ps(vec3_sq(oc_x, oc_y, oc_z), _mm_mul_ps(sph_r, sph_r));

            __m128 disc = _mm_sub_ps(_mm_mul_ps(b, b), _mm_mul_ps(_mm_mul_ps(a, c), four_const));
            __m128 sqrt_disc = _mm_sqrt_ps(disc);

            __m128 t = _mm_mul_ps(_mm_add_ps(sqrt_disc, b), inv2a); // (-b - sqrt(disc))/(2*a)

            for (auto j = 0; j < 4; ++j) {
                if (t[j] > 0 && t[j] < t_last) {
                    t_last = t[j];
                    o_last = spheres[i + j];
                }
            }
        }
    }

    // compute spheres on tail the standard way (maximum 3)
    for (auto i = spheres.size() - spheres.size()%4; i < spheres.size(); ++i) {
        auto t = spheres[i]->hit(r);
        if (t > 0 && t < t_last) {
            t_last = t;
            o_last = spheres[i];
        }
    }
}
