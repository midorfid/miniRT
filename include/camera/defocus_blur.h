#ifndef DEFOCUS_BLUR_H
#define DEFOCUS_BLUR_H

typedef struct defocus_blur_s {
    double  defocus_angle;

    vec3_t  defocus_disk_u;
    vec3_t  defocus_disk_v;
} defocus_blur_t;


#endif