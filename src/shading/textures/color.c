#include "../../include/render/color.h"

double  linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return (sqrt(linear_component));

    return (0);
}

static inline double reinhard_tone_map(double value) {
    return value / (value + 1.0);
}
int     return_color(color_t pixel_color) {
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;
    
    r = reinhard_tone_map(r);
    g = reinhard_tone_map(g);
    b = reinhard_tone_map(b);
    
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    
    
    int ir = (int)(256 * clamp(r, 0.000, 0.999));
    int ig = (int)(256 * clamp(g, 0.000, 0.999));
    int ib = (int)(256 * clamp(b, 0.000, 0.999));

    int color = (ib << 8) | (ig << 16) | (ir << 24)| (0xFF);
    return (color);
}

color_t color_in(double r, double g, double b) {
    color_t result = {.x = r, .y = g, .z = b};
    return (result);
}