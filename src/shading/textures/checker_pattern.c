#include "../../include/textures/checker_pattern.h"
#include "../../include/textures/texture_shared.h"

void        checker_pat_delete(texture_t *texture) {
    if (texture == NULL) {
        printf(" trying to delete checker_pattern texture which is NULL");
        return ;
    }
    if (texture->type != TEXTURE_TYPE_CHECKER_PATTERN) {
        printf("wrong texture type in delete, expected CHECKER_PATTERN");
        return ;
    }
    checker_pattern_t *todelete = (checker_pattern_t *)texture;
    texture_t_delete(todelete->even);
    texture_t_delete(todelete->odd);
    free(todelete);
}

color_t     checker_pat_getvalue(const texture_t *texture, double u, double v, const point3_t *p) {
    if (texture == NULL || texture->type != TEXTURE_TYPE_CHECKER_PATTERN || !p) {
        printf(" checker_pat_getvalue() failed");
        return color_in(0,0,0);
    }
    checker_pattern_t *cp_texture = (checker_pattern_t *)texture; 
    int x,y,z;
    x = (int)floor(cp_texture->scale * p->x);
    y = (int)floor(cp_texture->scale * p->y);
    z = (int)floor(cp_texture->scale * p->z);

    bool is_even = (x + y + z) % 2 == 0;
    return is_even ? texture_t_get_value(cp_texture->even,u,v,p) :
                texture_t_get_value(cp_texture->odd,u,v,p);
}

texture_t   *checker_pat_new(double scale, texture_t *even, texture_t *odd) {
    if (even == NULL || odd == NULL) {
        printf("checker new failed");
        return NULL;
    }
    checker_pattern_t   *new_texture = calloc(1, sizeof(checker_pattern_t));
    if (new_texture == NULL) {
        printf("checker new failed");
        return NULL;
    }
    new_texture->even = even;
    new_texture->odd = odd;
    new_texture->scale = scale;
    texture_t_innit(&new_texture->base, TEXTURE_TYPE_CHECKER_PATTERN, checker_pat_delete, checker_pat_getvalue);
    return (texture_t *)new_texture;
}

texture_t   *checker_pat_new_with_colour(double scale, color_t color_even, color_t color_odd) {
    return checker_pat_new(scale, solid_colour_new(color_even), solid_colour_new(color_odd));
}
