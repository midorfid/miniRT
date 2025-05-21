#ifndef PERLIN_H
#define PERLIN_H

#include "texture_shared.h"

#define PERLIN_POINT_C 256

typedef struct perlin_s {
    texture_t   base;
    
    double  random_float[PERLIN_POINT_C];
    
    int     x_perm[PERLIN_POINT_C];
    int     y_perm[PERLIN_POINT_C];
    int     z_perm[PERLIN_POINT_C];

    texture_t_free_met  delete;
    texture_t_value_met getvalue;
} perlin_t;

static void    perlin_scramble(const texture_t *texture, const point3_t *p);

static double    perlin_noise(const texture_t *texture, const point3_t *p);

static void    perlin_permute(int *p);

static void    perlin_generate_perm(int *p_axis);

void        perlin_delete(texture_t *texture);

color_t     perlin_getvalue(const texture_t *texture, double u, double v, const point3_t *p);

texture_t   *perlin_new();



#endif