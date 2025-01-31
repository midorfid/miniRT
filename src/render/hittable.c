#include "../../include/render/hittable_shared.h"

void    hittable_innit(hittable_t *hittable, hittable_type_t type, hittable_t_hit_met hit_met) {
    hittable->type = type;
    hittable->refcount = 1;
    hittable->hit = hit_met;
}

bool    hittable_t_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    return hittable->hit(hittable, ray, tmin, tmax, rec);
}
