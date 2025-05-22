#include "../../include/textures/perlin.h"

static void    perlin_scramble(const texture_t *texture, const point3_t *p) {
    perlin_t *perlin = (perlin_t *)texture;
    for (int i = 0;i < PERLIN_POINT_C; ++i) {
        perlin->randomvec[i] = vec3_random_limits(-1.0, 1.0);
    }
    perlin_generate_perm(perlin->x_perm);
    perlin_generate_perm(perlin->y_perm);
    perlin_generate_perm(perlin->z_perm);
}

static double   perlin_interp(vec3_t scalar[2][2][2], double raw_u, double raw_v, double raw_w) {
    double accum = 0.0;
    // Hermitian Smoothing ("fade function")
    double u_sm = raw_u * raw_u*(3 - 2*raw_u);
    double v_sm = raw_v * raw_v*(3 - 2*raw_v);
    double w_sm = raw_w * raw_w*(3 - 2*raw_w);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                vec3_t    weight_v = vec3(raw_u - i, raw_v - j, raw_w - k);
                accum += (i*u_sm + (1 - u_sm)*(1 - i)) *
                        (j*v_sm + (1 - v_sm)*(1 - j)) *
                        (k*w_sm + (1 - w_sm)*(1 - k)) *
                        vec3_dot(scalar[i][j][k], weight_v);
            }
        }
    }
    return (accum);
}

static double    perlin_noise(const texture_t *texture, const point3_t *p) {
    perlin_t *perlin = (perlin_t *)texture;

    //fractional part of p
    double raw_u,raw_v,raw_w;
    raw_u = p->x - floor(p->x);
    raw_v = p->y - floor(p->y);
    raw_w = p->z - floor(p->z);

    // cords for base bottom down left angle
    int i,j,k;
    i = (int)floor(p->x);
    j = (int)floor(p->y);
    k = (int)floor(p->z);

    vec3_t scalar[2][2][2];
    for (int di = 0; di < 2; ++di) {
        for (int dj = 0; dj < 2; ++dj) {
            for (int dk = 0; dk < 2; ++dk) {
                // assigning pseudo-random vec to each angle of int cube
                scalar[di][dj][dk] = perlin->randomvec[perlin->x_perm[i + di & 255] ^
                    perlin->y_perm[j + dj & 255] ^ perlin->z_perm[k + dk] & 255];
            }
        }
    }

    return perlin_interp(scalar, raw_u, raw_v, raw_w);
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

static double      noise_turbulance(const texture_t *texture, point3_t p, int depth) {
    double accum = 0.0;
    point3_t temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; ++i) {
        accum  += weight * perlin_noise(texture, &temp_p);
        weight *= 0.5;
        temp_p = vec3_scaled_return(temp_p, 2.0);
    }
    return fabs(accum);
}

color_t     perlin_getvalue(const texture_t *texture, double u, double v, const point3_t *p) {
    perlin_t    *temp_perlin = (perlin_t *)texture;

    // point3_t scaled_p = vec3_scaled_return(*p, temp_perlin->intensity);
    perlin_scramble(texture, p);

    double perlin = perlin_noise(texture, p);

    return vec3_scaled_return(color_in(0.5, 0.5, 0.5), 1 + sin(temp_perlin->intensity * p->z +  10 * noise_turbulance(texture, *p, 7)));
}
texture_t   *perlin_new(double intensity) {
    perlin_t *new_perlin = calloc(1, sizeof(perlin_t));
    
    new_perlin->intensity = intensity;

    texture_t_innit(&new_perlin->base, TEXTURE_TYPE_PERLIN_NOISE, perlin_delete, perlin_getvalue);
    return  (texture_t *)new_perlin;
}
