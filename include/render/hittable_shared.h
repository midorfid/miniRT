#ifndef HITTABLE_SHARED_H
#define HITTABLE_SHARED_H

#include "hittable.h"
#include "aabb.h"
#include "material_shared.h"

// #define PI (double)3.1415926535897932385

typedef enum hittable_type_e
{
    HITTABLE_TYPE_UNKNOWN = -1,
    HITTABLE_TYPE_SHPERE = 0,
    HITTABLE_TYPE_MOVING_SHPERE = 1,
    HITTABLE_TYPE_BVH_NODE = 2,
    HITTABLE_TYPE_QUAD = 3,
    HITTABLE_TYPE_BOX = 4,
} hittable_type_t;


typedef bool    (*hittable_t_hit_met)(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);
typedef bool    (*hittable_t_bb_met)(const hittable_t *hittable, double time0, double time1, aabb_t *bbox);
typedef void    (*hittable_t_delete_met)(hittable_t *hittable);

typedef int     (*hittable_t_compare_met)(const void *a, const void *b);

struct hittable_s {
    hittable_type_t         type;
    int                     refcount;

    hittable_t_hit_met      hit;
    hittable_t_bb_met       bb;
    hittable_t_delete_met   delete;
};

void    hittable_innit(hittable_t *hittable, hittable_type_t type, hittable_t_hit_met hit, hittable_t_bb_met bb, hittable_t_delete_met delete);


#endif