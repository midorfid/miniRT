#ifndef MV_SPHERE_H
#define MV_SPHERE_H

#include "sphere.h"

typedef struct mv_sphere_s {
    hittable_t  base;
    
    point3_t    center_start;
    point3_t    center_end;

    double      time_start;
    double      time_end;

    double      radius;
    material_t  *material;
} mv_sphere_t;

static mv_sphere_t          mv_sphere_init(point3_t center_start, point3_t center_end, double time_start, double time_end, double radius, material_t *material);

hittable_t                  *mv_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end, double radius, material_t *material);

static bool                 mv_sphere_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static point3_t             get_center_at_time (const mv_sphere_t *sphere, double t);

bool                        mv_sphere_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bb);

void                        mv_sphere_delete(hittable_t *hittable);

#endif