#ifndef LENS_H
#define LENS_H

#include "./render/vec3.h"

typedef struct defocus_blur_s {
    double  defocus_angle;
    double  focus_dist;

    vec3_t  defocus_disk_u;
    vec3_t  defocus_disk_v;
} defocus_blur_t;

#endif
