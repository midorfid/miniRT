#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "hit_record.h"
#include "aabb.h"
#include <stdbool.h>

typedef struct hittable_s hittable_t;

bool            hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

hittable_t     *hittable_claim(hittable_t *hittable);

bool            hittable_t_bb(const hittable_t *hittable, double time0, double time1, aabb_t *bbox);

static bool     hit_base(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    return false;
}

static void     delete_base(hittable_t *hittable) {
    free(hittable);
}

static bool     bb_base(const hittable_t *hittable, double time0, double time1, aabb_t *bbox) {
    return false;
}

#endif