#ifndef CHECKER_PATTERN_H
#define CHECKER_PATTERN_H

#include "texture.h"
#include "solid_colour.h"

typedef struct checker_pattern_s {
    texture_t   base;

    double      scale;

    texture_t   *even;
    texture_t   *odd;
} checker_pattern_t;

void        checker_pat_delete(texture_t *texture);

color_t     checker_pat_getvalue(const texture_t *texture, double u, double v, const point3_t *p);

texture_t   *checker_pat_new(double scale, texture_t *even, texture_t *odd);

texture_t   *checker_pat_new_with_colour(double scale, color_t color_even, color_t color_odd);

#endif