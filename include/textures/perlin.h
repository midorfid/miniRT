#ifndef PERLIN_H
#define PERLIN_H

#include "texture_shared.h"

#define PERLIN_POINT_C 256

typedef struct perlin_s {
    texture_t   base;
    
    vec3_t  randomvec[PERLIN_POINT_C];
    
    int     x_perm[PERLIN_POINT_C];
    int     y_perm[PERLIN_POINT_C];
    int     z_perm[PERLIN_POINT_C];

    double  intensity;
    
    texture_t_free_met  delete;
    texture_t_value_met getvalue;
} perlin_t;
//scalar is a pseudo-random vec of each angle that forms a cube surrouding point p
// u,v,w are the fractional part of our point between [0:1]
static double   perlin_interp(vec3_t scalar[2][2][2], double raw_u, double raw_v, double raw_w);

static void    perlin_scramble(const texture_t *texture, const point3_t *p);

static double    perlin_noise(const texture_t *texture, const point3_t *p);

static void    perlin_permute(int *p);

static void    perlin_generate_perm(int *p_axis);

void        perlin_delete(texture_t *texture);

color_t     perlin_getvalue(const texture_t *texture, double u, double v, const point3_t *p);

texture_t   *perlin_new(double intensity);



#endif