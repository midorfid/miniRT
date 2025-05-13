#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"

typedef struct aabb_s {
    point3_t min;
    point3_t max;
} aabb_t;

bool                aabb_hit(const aabb_t *aabb, double tmin, double tmax, const ray_t *ray);

aabb_t              aabb_surrounding_bbox(const aabb_t *a, const aabb_t *b);

#endif