#include "../../include/render/hittable_list.h"
#include "../../include/render/hittable.h"
#include <stdlib.h>
#include <assert.h>

hittable_list_t     *hittable_list_innit(size_t capacity) {
    hittable_list_t     *list;

    list = calloc(1, sizeof(hittable_list_t));
    if(list == NULL) {
        printf("Memory allocation failed");
        return (NULL);
    }
    list->hittables = calloc(capacity, sizeof(hittable_t *));
    if(list->hittables == NULL) {
        printf("Memory allocation failed");
        return (NULL);
    }
    list->capacity = capacity;
    list->size = 0;
    return (list);
}

void    hittable_list_add(hittable_list_t *list, hittable_t *hittable) {
    if (list->size > list->capacity) {
        list->hittables = realloc(list->hittables, list->capacity * 2 * sizeof(hittable_t *));
        if(list->hittables == NULL) {
            printf("Memory allocation failed");
        }
        list->capacity *= 2;
    }
    list->hittables[list->size++] = hittable;
}

bool    hittable_list_hit_test(const ray_t *ray, const hittable_list_t *list, double tmin, double tmax, hit_record_t *rec) {
    assert(NULL != list);
    assert(NULL != ray);

    if (NULL == list->hittables)
        return false;
    bool            hit_occured = false;
    hit_record_t    temp_rec;
    double          closest_t_so_far = tmax;

    for (int i = 0;i < list->size; ++i) {
        if (hittable_t_hit(list->hittables[i], ray, tmin, closest_t_so_far, &temp_rec)) {
            hit_occured = true;
            closest_t_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }
    return (hit_occured);
}

hittable_t          **get_hittables(const hittable_list_t *list) {
    return list->hittables;
}
