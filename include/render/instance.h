#ifndef INSTANCE_H
#define INSTANCE_H

#include "hittable_shared.h"
#include "matrix3.h"

#define RAY 0
#define BB 1

typedef struct instance_s {
    hittable_t  base;

    hittable_t  *hittable;

    vec3_t      offset;          
    matrix3_t   rotate_transform_m[2];
    matrix3_t   scale_transform_m[2];

    aabb_t      bbox;
} instance_t;

void            instance_delete(hittable_t *hittable);

hittable_t      *instance_new(hittable_t *hittable);

static bool     instance_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static bool     instance_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bbox);

void            instance_translate(hittable_t *hittable, vec3_t offset);

void            instance_rotate_y(hittable_t *hittable, double angle);

void            instance_uniform_scale(hittable_t *hittable, double uniform_scale);

void            instance_scale(hittable_t *hittable, vec3_t scale_vector);

#endif