#include "../../include/textures/perlin.h"

static void    perlin_scramble(const texture_t *texture, const point3_t *p) {
    perlin_t *perlin = (perlin_t *)texture;
    for (int i = 0;i < PERLIN_POINT_C; ++i) {
        perlin->random_float[i] = random_double_nolimits();
    }
    perlin_generate_perm(perlin->x_perm);
    perlin_generate_perm(perlin->y_perm);
    perlin_generate_perm(perlin->z_perm);
}

static double   trilinear_interp(double scalar[2][2][2], double u, double v, double w) {
    double accum = 0.0;
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                accum += (i*u + (1 - u)*(1 - i)) *
                        (j*v + (1 - v)*(1 - j)) *
                        (k*w + (1 - w)*(1 - k)) *
                        scalar[i][j][k];
            }
        }
    }
    return (accum);
}

static double    perlin_noise(const texture_t *texture, const point3_t *p) {
    perlin_t *perlin = (perlin_t *)texture;

    //fractional part of p
    double u,v,w;
    u = p->x - floor(p->x);
    v = p->y - floor(p->y);
    w = p->z - floor(p->z);

    // cords for base bottom down left angle
    int i,j,k
    i = (int)floor(p->x);
    j = (int)floor(p->y);
    k = (int)floor(p->z);

    double scalar[2][2][2];
    for (int di = 0; di < 2; ++di) {
        for (int dj = 0; dj < 2; ++dj) {
            for (int dk = 0; dk < 2; ++dk) {
                // assigning pseudo-random double to each angle of int cube
                scalar[di][dj][dk] = perlin->random_float[perlin->x_perm[i + di & 255] ^
                    perlin->y_perm[j + dj & 255] ^ perlin->z_perm[k + dk] & 255];
            }
        }
    }

    return trilinear_interp(scalar, u, v, w);
}

static void    perlin_permute(int *p) {
    for (int i = PERLIN_POINT_C-1;i > 0; --i) {
        int target = my_random_int(0, i);
        int temp = p[i];
        p[i] = target;
        p[target] = temp;
    }
}

static void    perlin_generate_perm(int *p) {
    for (int i = 0; i < PERLIN_POINT_C; ++i) {
        p[i] = i;
    }
    perlin_permute(p);
}
void        perlin_delete(texture_t *texture) {
    if (texture == NULL || texture->type != TEXTURE_TYPE_PERLIN_NOISE) {
        printf("failed at perlin_delete()");
        return ;
    }
    perlin_t *perlin = (perlin_t *)texture;
    free(perlin);
}
color_t     perlin_getvalue(const texture_t *texture, double u, double v, const point3_t *p) {
    perlin_scramble(texture, p);
    return vec3_scaled_return(color_in(1,1,1), perlin_noise(texture, p));
}
texture_t   *perlin_new() {
    perlin_t *new_perlin = calloc(1, sizeof(perlin_t));
    
    texture_t_innit(&new_perlin->base, TEXTURE_TYPE_PERLIN_NOISE, perlin_delete, perlin_getvalue);
    return  (texture_t *)new_perlin;
}
