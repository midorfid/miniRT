#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "hittable_shared.h"
#include "../textures/texture_shared.h"

typedef struct const_medium_s {
    hittable_t  base;

    hittable_t  *boundary;
    material_t  *phase_function;

    double      inv_neg_density;
} const_medium_t;

bool        const_medium_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

void        const_medium_delete(hittable_t *hittable);

bool        const_medium_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bbox);

hittable_t  *const_medium_new_with_tex(hittable_t *boundary, double density, texture_t *texture);

hittable_t  *const_medium_new_with_colour(hittable_t *boundary, double density, double albedo);

#endif