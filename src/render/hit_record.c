#include "../../include/render/hit_record.h"

static inline void  set_front_face(const ray_t *r, const vec3_t *outward_normal, hit_record_t *rec) {
    rec->front_face = vec3_dot(r->dir, *outward_normal) < 0;
    rec->normal = rec->front_face ? *outward_normal : vec3_negative(outward_normal);
}
