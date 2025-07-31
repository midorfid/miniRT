#ifndef ONB_H
#define ONB_H

#include "render/vec3.h"

#define W 2
#define V 1
#define U 0

typedef struct onb_s {
    vec3_t axis[3];
} onb_t;

static inline onb_t   onb_init(const vec3_t *n) {
    onb_t   uvw;

    uvw.axis[2] = vec3_normalize(*n);
    vec3_t a = fabs(uvw.axis[2].x) > 0.9 ? vec3(0,1,0) : vec3(1,0,0);
    uvw.axis[1] = vec3_normalize(vec3_cross(uvw.axis[2], a));
    uvw.axis[0] = vec3_cross(uvw.axis[2], uvw.axis[1]);

    return uvw;
}

static inline vec3_t  onb_transform(const vec3_t *v, const onb_t *uvw) {
    vec3_t temp = vec3_sum(vec3_multi(v[0], uvw->axis[0]), vec3_multi(v[1], uvw->axis[1]));
    return vec3_sum(temp, vec3_multi(v[2], uvw->axis[2]));
}

#endif