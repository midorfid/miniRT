#include "../../include/render/color.h"

int     return_color(color_t pixel_color) {
    int ir = (int)(255.999 * pixel_color.x);
    int ig = (int)(255.999 * pixel_color.y);
    int ib = (int)(255.999 * pixel_color.z);

    int color = (ib << 8) | (ig << 16) | (ir << 24)| (0xFF);

    return (color);
}

static inline color_t color_in(double r, double g, double b) {
    color_t result = {.x = r, .y = g, .z = b};
    return (result);
}