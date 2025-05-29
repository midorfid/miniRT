#ifndef INSTANCE_H
#define INSTANCE_H

#include "hittable_shared.h"

typedef struct matrix3_s {
    double matrix[3][3];
} matrix3_t;

typedef struct instance_s {
    hittable_t  base;

    hittable_t  *hittable;

    vec3_t      offset;
    matrix3_t   ray_rotation_m;
    matrix3_t   bb_rotation_m;

    aabb_t      bbox;
} instance_t;

static inline matrix3_t     matrix_default(void) {
    matrix3_t   result = {0};
    result.matrix[0][0] = result.matrix[1][1] = result.matrix[2][2] = 1;
    return result;
}

static inline matrix3_t     matrix_rotation_y(double angle) {
    double      theta_cos = cos(DEG_TO_RAD(angle));
    double      theta_sin = sin(DEG_TO_RAD(angle));
    // cos(A+B)=cosAcosB−sinAsinB
    // sin(A+B)=sinAcosB+cosAsinB
    matrix3_t result = { .matrix =
    {
        theta_cos, 0, theta_sin,
        0, 1, 0,
        -theta_sin, 0, theta_cos,
    }};
    return result;
}

void            instance_delete(hittable_t *hittable);

hittable_t      *instance_new(hittable_t *hittable);

static bool     instance_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static bool     instance_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bbox);

void            instance_translate(hittable_t *hittable, vec3_t offset);

void            instance_rotate_y(hittable_t *hittable, double angle);

vec3_t          vec3_multi_by_matrix(const vec3_t *a, const matrix3_t *matrix);

#endif