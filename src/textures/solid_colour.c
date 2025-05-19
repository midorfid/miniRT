#include "../../include/textures/solid_colour.h"

void        solid_colour_delete(texture_t *texture) {
    if (texture == NULL) {
        printf(" texture is NULL ");
        return ;
    }
    if (texture->type != TEXTURE_TYPE_SOLID_COLOUR) {
        printf(" (delete)wrong type for solic_color ");
        return ;
    }
    free(texture);
}

color_t     solid_colour_getvalue(const texture_t *texture, double u, double v, const point3_t *p) {
    solid_colour_t *sc_texture = (solid_colour_t *)texture;
    return sc_texture->colour;
}

texture_t   *solid_colour_new(color_t color) {
    solid_colour_t *new_texture = calloc(1, sizeof(solid_colour_t));
    if (new_texture == NULL) {
        printf(" solid_colour alloc failed ");
        return (texture_t*)new_texture;
    }
    new_texture->colour = color;
    texture_t_innit(&new_texture->base, TEXTURE_TYPE_SOLID_COLOUR, solid_colour_delete, solid_colour_getvalue);
    return (texture_t*)new_texture;
}

texture_t   *solid_colour_new_with_components(double r, double g, double b) {
    return solid_colour_new(color_in(r,g,b));
}
