#include "../../include/render/material_diffuse_light.h"

bool    diffuse_light_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    
}

color_t    diffuse_light_emmit(const material_t *material, double u, double v, const point3_t *point) {
    if (material == NULL || material->type != MATERIAL_TYPE_DIFFUSE_LIGHT) {
        printf("diffuse_light_emmit() failed");
        return color_in(0, 0, 0);
    }
    diffuse_light_t *diff_light = (diffuse_light *)material;

    return diff_light->texture->get_value(diff_light->texture, u, v, p);
}

void    diffuse_light_delete(material_t *material) {
    if (material == NULL || material->type != MATERIAL_TYPE_DIFFUSE_LIGHT) {
        printf("diffuse_light_delete() failed");
        return ;
    }
    diffuse_light_t *diff_light = (diffuse_light *)material;
    
    texture_t_delete(diff_light->texture);
    free(diff_light);
}

material_t      *diffuse_light_new_with_tex(texture_t *texture, double intensity) {
    diffuse_light_t *diff_light_new = calloc(1, sizeof(diffuse_light_t));
    if (diff_light_new == NULL) {
        printf("diffuse_light_new() failed");
        return NULL;
    }
    diff_light_new = 
    {
        .texture = texture,
        .intensity = intensity,
    };
    material_base_innit(&diff_light.base, MATERIAL_DIFFUSE_LIGHT_H, diffuse_light_scatter, diffuse_light_emmit, diffuse_light_delete);

    return (material_t *)diff_light_new;
}

material_t      *diffuse_light_new_with_colour(color_t albedo, double intensity) {
    return diffuse_light_new_with_tex(solid_colour_new(albedo), intensity);
}
