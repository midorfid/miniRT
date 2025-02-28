#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray_s {
    point3_t      orig;
    vec3_t        dir;
    // double      t;
} ray_t;

static inline ray_t ray(point3_t orig, vec3_t dir) {
    ray_t result = {.orig = orig, .dir = dir};
    return (result);
}

static inline point3_t    ray_at(point3_t orig, vec3_t dir, double t) {
    return (vec3_sum(orig, vec3_scaled_return(dir, t)));
}

static inline vec3_t sample_square() {
    return vec3(random_double_nolimits() - 0.5, random_double_nolimits() - 0.5, 0);
}

static inline ray_t get_ray(int i, int j, point3_t pixel00_loc, vec3_t pixel_delta_u, vec3_t pixel_delta_v, point3_t center) {
    // Constructs a ray originating from the origin and directed at ramdomly
    // sampled point around the pixel location i, j
    vec3_t offset = sample_square();
    point3_t pixel_sample = vec3_sum(vec3_sum(pixel00_loc, vec3_scaled_return(pixel_delta_u, (i + offset.x))),
                                     vec3_scaled_return(pixel_delta_v, (j + offset.y)));
    point3_t ray_origin = center;
    point3_t ray_dir = vec3_sub_return(pixel_sample, ray_origin);

    return ray(ray_origin, ray_dir);
}

#endif
