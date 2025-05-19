#ifndef SOLID_COLOUR_H
#define SOLID_COLOUR_H

#include "texture_shared.h"

typedef struct solid_colour_s {
    texture_t   base;

    color_t     colour;
} solid_colour_t;

void        solid_colour_delete(texture_t *texture);

color_t     solid_colour_getvalue(const texture_t *texture, double u, double v, const point3_t *p);

texture_t   *solid_colour_new(color_t color);

texture_t   *solid_colour_new_with_components(double r, double g, double b);

#endif