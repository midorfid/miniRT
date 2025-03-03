#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vec3.h"
#include "ray.h"
#include <stdbool.h>

typedef struct material_s material_t;

typedef struct hit_record_s {
    point3_t        p;
    vec3_t          normal;
    double          t;
    material_t      mat;

    bool            front_face;
} hit_record_t;

void  set_front_face(const ray_t *r, const vec3_t *outward_normal, hit_record_t *rec);

#endif
