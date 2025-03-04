#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

// Color layer for vec3_t
typedef vec3_t color_t;
#define color(r, g, b) vec3((r), (g), (b))

color_t color_in(double r, double g, double b);

int    return_color(color_t pixel_color);

#endif
