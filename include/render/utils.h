#ifndef UTILS_H
#define UTILS_H

#include "hittable.h"

#define PI (double)3.1415926535897932385

typedef enum hittable_type_e
{
    HITTABLE_TYPE_UNKNOWN = -1,
    HITTABLE_TYPE_SHPERE = 0,
} hittable_type_t;

typedef struct hittable_s hittable_t;

typedef bool (*hittable_t_hit_met)(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

struct hittable_s {
    hittable_type_t     type;
    int                 refcount;

    hittable_t_hit_met hit;
};



#endif