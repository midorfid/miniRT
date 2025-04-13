#ifndef RENDER_PLANE_H
#define RENDER_PLANE_H

typedef struct render_plane_s {
    vec3_t  pixel_delta_u;
    vec3_t  pixel_delta_v;

    point3_t    pixel00_loc;
} render_plane_t;


#endif