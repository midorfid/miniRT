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
    double x = reinhard_tone_map(pixel_color.x);
    double y = reinhard_tone_map(pixel_color.y);
    double z = reinhard_tone_map(pixel_color.z);

    
    double r = linear_to_gamma(x);
    double g = linear_to_gamma(y);
    double b = linear_to_gamma(z);
    
    
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