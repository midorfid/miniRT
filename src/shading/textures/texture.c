#include "../../include/textures/texture.h"
#include "../../include/textures/texture_shared.h"

static void    texture_t_free_default(texture_t *texture) {
    printf("default free for texture");
    return ;
}

static color_t  texture_t_get_value_default(const texture_t *texture, double u, double v, const point3_t *p) {
    printf("default value for texture");
    return color_in(0.0, 0.0, 0.0);
}

void    texture_t_innit(texture_t *base, texture_type_t type, texture_t_free_met free, texture_t_value_met get_value) {
    if (base == NULL) {
        printf("texture init failed base == NULL");
        return ;
    }

    base->refcount = 1;
    base->type = type;

    if (free == NULL)
        free = texture_t_free_default; 
    base->free = free;

    if (get_value == NULL)
        get_value = texture_t_get_value_default;
    base->get_value = get_value;
}

void    texture_t_delete(texture_t *texture) {
    if (texture == NULL) {
        printf("texture == NULL in basic delete");
        return ;
    }
    if (--texture->refcount > 0) {
        printf ("texture's refcount > 0, can't delete");
        return ;
    }
    texture->free(texture);
}

color_t texture_t_get_value(const texture_t *texture, double u, double v, const point3_t *p) {
    if (texture == NULL) {
        printf(" texture == NULL in texture_t_get_value()");
        return color_in(0,0,0);
    }
    return texture->get_value(texture, u, v, p);
}
