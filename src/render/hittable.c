#include "../../include/render/hittable_shared.h"

void    hittable_innit(hittable_t *hittable, hittable_type_t type, hittable_t_hit_met hit, hittable_t_bb_met bb, 
                    hittable_t_delete_met delete, hittable_t_pdf_value_met pdf_value, hittable_t_random_met random) {
    hittable->type = type;
    hittable->refcount = 1;

    if (hit == NULL)
        hit = hit_base;
    hittable->hit = hit;
    if (bb == NULL)
        bb = bb_base;
    hittable->bb = bb;
    if (delete == NULL)
        delete = delete_base;
    hittable->delete = delete;
    if (pdf_value == NULL)
        pdf_value = pdf_value_base;
    hittable->pdf_value = pdf_value;
    if (random == NULL)
        random = random_base;
    hittable->random = random;
}

bool    hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    return hittable->hit(hittable, ray, tmin, tmax, rec);
}

hittable_t     *hittable_claim(hittable_t *hittable) {
    hittable->refcount++;

    return hittable;
}

bool hittable_t_bb(const hittable_t *hittable, double time0, double time1, aabb_t *bbox) {
    if (hittable == NULL) {
        printf("hittable is NULL");
        return false;
    }
    return hittable->bb(hittable, time0, time1, bbox);
}

double          hittable_pdf_value(const hittable_t *hittable, const point3_t *origin, const vec3_t *dir) {
    return hittable->pdf_value(hittable, origin, dir);
}

vec3_t          hittable_random(const hittable_t *hittable, const point3_t *origin) {
    return hittable->random(hittable, origin);
}



// A negative integer if the element pointed to by a should come before the element pointed to by b.
int                box_x_compare(const void *a, const void *b) {
    aabb_t a_bbox,b_bbox;

    if (!hittable_t_bb(*(const hittable_t *const *)a, 0, 0, &a_bbox) ||
        !hittable_t_bb(*(const hittable_t *const *)b, 0, 0, &b_bbox))
    {
        printf(" bbox x axis compare failed ");
        return 0;
    }
    if (a_bbox.min.x < b_bbox.min.x)
        return -1;
    else if (a_bbox.min.x > b_bbox.min.x)
        return 1;
    return 0;
}
// Zero if the elements pointed to by a and b are considered equal for sorting purposes.
int                box_y_compare(const void *a, const void *b) {
    aabb_t a_bbox,b_bbox;

    if (!hittable_t_bb(*(const hittable_t *const *)a, 0, 0, &a_bbox) ||
        !hittable_t_bb(*(const hittable_t *const *)b, 0, 0, &b_bbox))
    {
        printf(" bbox y axis compare failed ");
        return 0;
    }
    if (a_bbox.min.y < b_bbox.min.y)
        return -1;
    else if (a_bbox.min.y > b_bbox.min.y)
        return 1;
    return 0;
}
// A positive integer if the element pointed to by a should come after the element pointed to by b.
int                box_z_compare(const void *a, const void *b) {
    aabb_t a_bbox,b_bbox;

    if (!hittable_t_bb(*(const hittable_t *const *)a, 0, 0, &a_bbox) ||
        !hittable_t_bb(*(const hittable_t *const *)b, 0, 0, &b_bbox))
    {
        printf(" bbox z axis compare failed ");
        return 0;
    }
    if (a_bbox.min.z < b_bbox.min.z)
        return -1;
    else if (a_bbox.min.z > b_bbox.min.z)
        return 1;
    return 0;
}

void            hittable_delete(hittable_t *hittable) {
    if (hittable == NULL || --hittable->refcount > 0) {
        printf("hittable_delete() failed");
        return ;
    }
    hittable->delete(hittable);
}
