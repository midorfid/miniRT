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

static double    perlin_noise(const texture_t *texture, const point3_t *p) {
    perlin_t *perlin = (perlin_t *)texture;

    int i = (int)(4 * p->x) & 255;
    int j = (int)(4 * p->y) & 255;
    int k = (int)(4 * p->z) & 255;

    return perlin->random_float[i ^ j ^ k];
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
