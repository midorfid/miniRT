#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "hit_record.h"
#include "aabb.h"
#include <stdbool.h>
#include "../textures/texture.h"

typedef struct hittable_s hittable_t;

bool            hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

hittable_t     *hittable_claim(hittable_t *hittable);

bool            hittable_t_bb(const hittable_t *hittable, double time0, double time1, aabb_t *bbox);

void            hittable_delete(hittable_t *hittable);

double          hittable_pdf_value(const hittable_t *hittable, const point3_t *origin, const vec3_t *dir);

vec3_t          hittable_random(const hittable_t *hittable, const point3_t *origin);


static bool     hit_base(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    return false;
}

static void     delete_base(hittable_t *hittable) {
    free(hittable);
}

static bool     bb_base(const hittable_t *hittable, double time0, double time1, aabb_t *bbox) {
    return false;
}

static double   pdf_value_base(const hittable_t *hittable, const point3_t *origin, const vec3_t *dir) {
    return 0.0;
}

static vec3_t   random_base(const hittable_t *hittable, const point3_t *origin) {
    return vec3(1,0,0);
}

hittable_t                  *sphere_new(point3_t center, double radius, material_t *material);

hittable_t                  *quad_new(point3_t Q, vec3_t u, vec3_t v, material_t *material);

hittable_t                  *box_new(point3_t min, point3_t max, material_t *material);

hittable_t                  *const_medium_new_with_tex(hittable_t *boundary, double density, texture_t *texture);

hittable_t                  *const_medium_new_with_colour(hittable_t *boundary, double density, double albedo);

hittable_t                  *mv_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end, double radius, material_t *material);

#endif