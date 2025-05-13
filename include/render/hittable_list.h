#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

typedef struct hittable_list_s {
    hittable_t      **hittables;
    size_t          size;
    size_t          capacity;
} hittable_list_t;

hittable_list_t     *hittable_list_innit(size_t capacity);

void                hittable_list_add(hittable_list_t *list, hittable_t *hittable);

void                delete_hittable_list(hittable_list_t *list);

bool                hittable_list_hit_test(const ray_t *ray, const hittable_list_t *list, double tmin, double tmax, hit_record_t *rec);

hittable_t          **get_hittables(const hittable_list_t *list);

#endif