#ifndef TEXTURE_SHARED_H
#define TEXTURE_SHARED_H

#include "texture.h"
#include "../render/color.h"

typedef enum texture_type_s {
    TEXTURE_TYPE_NONE = -1,
    TEXTURE_TYPE_SOLID_COLOUR,
    TEXTURE_TYPE_CHECKER_PATTERN,
    TEXTURE_TYPE_PERLIN_NOISE,
} texture_type_t;

typedef void (*texture_t_free_met)(texture_t *texture);
typedef color_t (*texture_t_value_met)(const texture_t *texture, double u, double v, const point3_t *p);

struct texture_s {
    texture_type_t  type;
    int             refcount;

    texture_t_free_met      free;
    texture_t_value_met     get_value;
};

void    texture_t_innit(texture_t *base, texture_type_t type, texture_t_free_met free, texture_t_value_met get_value);


#endif