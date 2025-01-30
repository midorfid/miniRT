#ifndef HITTABLE_H
#define HITTABLE_H

#include "hittable.h"
#include "utils.h"
#include "ray.h"
#include "hit_record.h"
#include <stdbool.h>



void    hittable_innit(hittable_t *hittable, hittable_type_t type, hittable_t_hit_met hit);

bool    hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

#endif