#include "../../include/render/color.h"

int     return_color(color_t pixel_color) {
    int ir = (int)(256 * clamp(pixel_color.x, 0.000, 0.999));
    int ig = (int)(256 * clamp(pixel_color.y, 0.000, 0.999));
    int ib = (int)(256 * clamp(pixel_color.z, 0.000, 0.999));

    int color = (ib << 8) | (ig << 16) | (ir << 24)| (0xFF);
    return (color);
}

static inline color_t color_in(double r, double g, double b) {
    color_t result = {.x = r, .y = g, .z = b};
    return (result);
}