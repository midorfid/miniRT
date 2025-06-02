#include "../../include/render/material_isotropic.h"

static bool         isotropic_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    if (material == NULL || material->type != MATERIAL_TYPE_ISOTROPIC) {
        printf("isotropic_scatter() failed");
        return false;
    }
    isotropic_t *iso = (isotropic_t *)material;
    *scattered = ray(rec->p, vec3_random_unit_vec(), ray_in->time);
    *attenuation = texture_t_get_value(iso->albedo, rec->u, rec->v, &rec->p);

    return true;
}

static void         isotropic_delete(material_t *mat) {
    if (mat == NULL || mat->type != MATERIAL_TYPE_ISOTROPIC) {
        printf("isotropic_delete() failed");
        return ;
    }
    isotropic_t *todelete = (isotropic_t *)mat;
    texture_t_delete(todelete->albedo);
    free(todelete);
}

material_t          *isotropic_new_with_tex(texture_t *tex) {
    isotropic_t *new = calloc(1, sizeof(isotropic_t));
    if (new == NULL) {
        printf ("isotropic_new() failed");
        return NULL;
    }
    new->albedo = tex;
    material_base_innit(&new->base, MATERIAL_TYPE_ISOTROPIC, isotropic_scatter, NULL, isotropic_delete);
    return (material_t *)new;
}

material_t          *isotropic_new_with_colour(color_t albedo) {
    return isotropic_new_with_tex(solid_colour_new(albedo));
}
