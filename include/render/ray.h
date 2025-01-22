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


#endif
