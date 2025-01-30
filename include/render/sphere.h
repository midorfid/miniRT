#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

typedef struct sphere_s {
    hittable_t  base;
    
    point3_t    center;
    double      radius;
} sphere_t;

static inline sphere_t      sphere_init(point3_t center, double radius);

static inline bool          hit_sphere(sphere_t sphere, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static inline hittable_t    *sphere_new(point3_t center, double radius);

#endif
