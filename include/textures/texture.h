#ifndef TEXTURE_H
#define TEXTURE_H

#include "../render/color.h"

typedef struct texture_s texture_t;

void    texture_t_delete(texture_t *texture);

color_t texture_t_get_value(const texture_t *texture, double u, double v, const point3_t *p);

#endif