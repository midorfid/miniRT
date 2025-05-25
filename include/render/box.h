#ifndef MY_BOX_H
#define MY_BOX_H

#include "hittable_shared.h"
#include "hittable_list.h"
#include "quad.h"
#include "hittable.h"

typedef struct my_box_s {
    hittable_t          base;

    hittable_list_t     *sides;
    point3_t            min, max;
} my_box_t;

void    box_delete(hittable_t *box);

bool    box_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec);

bool    box_bb(const hittable_t *hittable, double tmin, double tmax, aabb_t *bbox);

static hittable_list_t     *box_sides_init(point3_t a, point3_t b, material_t *material);

static my_box_t        box_innit(point3_t min, point3_t max, material_t *material);

hittable_t      *box_new(point3_t min, point3_t max, material_t *material);

#endif