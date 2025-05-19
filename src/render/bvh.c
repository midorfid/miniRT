#include "../../include/render/bvh.h"
#include <stdlib.h>

static bool         bvh_node_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL) {
        printf("hittable is NULL");
        return false;
    }
    if (hittable->type != HITTABLE_TYPE_BVH_NODE) {
        printf("wrong type for bvh_node_hit");
        return false;
    }
    bvh_node_t *node = (bvh_node_t *)hittable;
    if (!aabb_hit(&node->box, tmin, tmax, ray))
        return false;
    bool hit_left = hittable_t_hit(node->left, ray, tmin, tmax, rec);
    bool hit_right = hittable_t_hit(node->right, ray, tmin, hit_left ? rec->t : tmax, rec);

    return (hit_left || hit_right);
}

static void         bvh_node_delete(hittable_t *hittable) {
    if (hittable == NULL || --hittable->refcount < 0) {
        printf(" hittable not deleted ");
        return ;
    }
    hittable->delete(hittable);
}

hittable_t          *bvh_node_new(const hittable_list_t *list, double time0, double time1) {
    return (bvh_make_node(get_hittables(list), 0, list->size, time0, time1));
}

static hittable_t   *bvh_make_node(hittable_t **hittables, size_t start, size_t end, double time0, double time1) {
    if (hittables == NULL) {
        printf("hittables list is NULL");
        return NULL;
    }
    bvh_node_t *result = calloc(1, sizeof(bvh_node_t));
    if (result == NULL) {
        printf(" bvh_make_node allocation failed ");
        return NULL;
    }
    size_t  object_span = end - start;
    int     axis = my_random_int(0,2);
    hittable_t_compare_met comparator = box_x_compare;
    if (axis = 1)
        comparator = box_y_compare;
    if (axis = 2)
        comparator = box_z_compare;
    if (object_span == 1)
        result->left = result->right = hittable_claim(hittables[start]); 
    else if (object_span == 2) {
        if (comparator(hittables + start, hittables + start + 1) < 0) {
            result->left = hittable_claim(hittables[start]);
            result->right = hittable_claim(hittables[start+1]);
        }
        else {
            result->left = hittable_claim(hittables[start+1]);
            result->right = hittable_claim(hittables[start]);
        }
    }
    else {
        qsort(result + start, object_span, sizeof(hittable_t), comparator);
        size_t mid = start + object_span / 2;
        result->left = bvh_make_node(hittables, start, mid, time0, time1);
        result->right = bvh_make_node(hittables, mid, end, time0, time1);
    }
    aabb_t box_left = {0}, box_right = {0};

    if (!hittable_t_bb(result->left, time0, time1, &box_left) ||
        !hittable_t_bb(result->right, time0, time1, &box_right))
    {
        printf(" bvh_make_node failed at hittable_t_bb()");
        return NULL;
    }
    result->box = aabb_surrounding_bbox(box_left, box_right);
    hittable_innit(&result->base, HITTABLE_TYPE_BVH_NODE, bvh_node_hit, bb_base, delete_base);
}
