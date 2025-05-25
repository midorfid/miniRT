#ifndef MY_QUAD_H
#define MY_QUAD_H

#include "hittable_shared.h"

typedef struct my_quad_s {
    hittable_t base;
    
    point3_t    Q; // Origin of the quad.
    vec3_t      u,v; // side vectors defining shape of the parallelogram.

    vec3_t      normal; // normalized normal vector of quadrilateral's plane.
    double      D; // This is a scalar constant. It influences the position of the plane in space, specifically its distance from the origin (0,0,0).

    vec3_t      w; // cached vector for calculating planar coords alpha, beta.

    material_t *material;
} my_quad_t;

static my_quad_t       quad_init(point3_t Q, vec3_t u, vec3_t v, material_t *material);

static bool         quad_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

hittable_t          *quad_new(point3_t Q, vec3_t u, vec3_t v, material_t *material);

void                quad_delete(hittable_t *hittable);

bool                quad_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_box);

static bool         is_interior(double a, double b, hit_record_t *rec);

#endif