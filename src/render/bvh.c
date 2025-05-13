#include "../../include/render/bvh.h"

static bool         bvh_node_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL) {
        printf("hittable is NULL")
        return false;
    }
    if (hittable->type != HITTABLE_TYPE_BVH_NODE) {
        printf("wrong type for bvh_node_hit")
        return false;
    }
    bvh_node_t *node = (bvh_node_t *)hittable;
    if (!aabb_hit(&node->box, tmin, tmax, ray))
        return false;
    bool hit_left = hittable_t_hit(node->left, ray, tmin, tmax, rec);
    bool hit_right = hittable_t_hit(node->right, tmin, hit_left ? rec->t : tmax, rec);

    return (hit_left || hit_right);
}

static void         bvh_node_delete(hittable_t *hittable);    

hittable_t          *bvh_node_new(const hittable_list_t *list, double time0, double time1);

static hittable_t   *bvh_make_node(hittable_t **hittables, size_t start, size_t end, double time0, double time1);
