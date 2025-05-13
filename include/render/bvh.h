#ifndef BVH_H
#define BVH_H

#include "hittable_shared.h"
#include "hittable_list.h"
#include "aabb.h"

typedef struct bvh_node_s {
    hittable_t  base;

    aabb_t      box;

    hittable_t  *right;
    hittable_t  *left;
} bvh_node_t;

static bool         bvh_node_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

static void         bvh_node_delete(hittable_t *hittable);    

hittable_t          *bvh_node_new(const hittable_list_t *list, double time0, double time1);

static hittable_t   *bvh_make_node(hittable_t **hittables, size_t start, size_t end, double time0, double time1);

#endif