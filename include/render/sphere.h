#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable_shared.h"

typedef struct sphere_s {
    hittable_t  base;
    
    point3_t    center;
    double      radius;
    material_t  *material;
} sphere_t;

static inline sphere_t      sphere_init(point3_t center, double radius, material_t *material);

hittable_t                  *sphere_new(point3_t center, double radius, material_t *material);

static inline bool          sphere_hit_test_generic(point3_t center, double radius, material_t *material, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static inline bool          sphere_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

#endif
