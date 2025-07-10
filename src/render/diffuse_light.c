#include "material_diffuse_light.h"
#include "../../include/textures/solid_colour.h"

bool    diffuse_light_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    return false;
}

color_t    diffuse_light_emmit(const material_t *material, double u, double v, const point3_t *point) {
    if (material == NULL || material->type != MATERIAL_TYPE_DIFFUSE_LIGHT) {
        printf("diffuse_light_emmit() failed");
        return color_in(0, 0, 0);
    }
    diffuse_light_t *diff_light = (diffuse_light_t *)material;

    return diff_light->texture->get_value(diff_light->texture, u, v, point);
}

void    diffuse_light_delete(material_t *material) {
    if (material == NULL || material->type != MATERIAL_TYPE_DIFFUSE_LIGHT) {
        printf("diffuse_light_delete() failed");
        return ;
    }
    diffuse_light_t *diff_light = (diffuse_light_t *)material;
    
    texture_t_delete(diff_light->texture);
    free(diff_light);
}

material_t      *diffuse_light_new_with_tex(texture_t *texture) {
    diffuse_light_t *diff_light_new = calloc(1, sizeof(diffuse_light_t));
    if (diff_light_new == NULL) {
        printf("diffuse_light_new() failed");
        return NULL;
    }
    diff_light_new->texture = texture;
    // diff_light_new->intensity = intensity;        
    material_base_innit(&diff_light_new->base, MATERIAL_TYPE_DIFFUSE_LIGHT, NULL, diffuse_light_emmit, diffuse_light_delete);

    return (material_t *)diff_light_new;
}

material_t      *diffuse_light_new_with_colour(color_t albedo) {
    return diffuse_light_new_with_tex(solid_colour_new(albedo));
}
