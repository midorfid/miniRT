#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "hit_record.h"
#include <stdbool.h>

typedef struct hittable_s hittable_t;

bool    hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

#endif