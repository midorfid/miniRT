#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"

typedef struct aabb_s {
    point3_t min;
    point3_t max;
} aabb_t;

bool                aabb_hit(const aabb_t *aabb, double tmin, double tmax, const ray_t *ray);

aabb_t              aabb_surrounding_bbox(aabb_t a, aabb_t b);

aabb_t              aabb_two_points(point3_t a, point3_t b);

int                box_x_compare(const void *a, const void *b);
int                box_y_compare(const void *a, const void *b);
int                box_z_compare(const void *a, const void *b);

#endif