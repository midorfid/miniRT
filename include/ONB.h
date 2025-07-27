#ifndef ONB_H
#define ONB_H

#include "render/vec3.h"

#define W 2
#define V 1
#define U 0

typedef struct onb_s {
    vec3_t axis[3];
} onb_t;

onb_t   onb_init(const vec3_t *n) {
    onb_t   uvw;

    uvw[2] = vec3_normalize(n);
    vec3_t a = fabs(uvw[2].x > 0.9) ? vec3(0,1,0) : vec3(1,0,0);
    uvw[1] = vec3_normalize(vec3_cross(uvw[2], a));
    uvw[0] = vec3_cross(uvw[2], uvw[1]);

    return uvw;
}

vec3_t  onb_transform(const vec3_t *v, const onb_t *uvw) {
    return vec3(v[0] * uvw[0], v[1] * uvw[1], v[2] * uvw[2]);
}

#endif