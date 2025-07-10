#ifndef MATERIAL_LAMBERTIAN_H
#define MATERIAL_LAMBERTIAN_H

#include "../../include/render/hit_record.h"
#include "../../include/render/ray.h"
#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"
#include "../../include/textures/solid_colour.h"

typedef struct lambertian_s
{
    material_t  base;
    
    texture_t   *texture;
} lambertian_t;

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void     mt_lambertian_delete(material_t *material);

material_t      *mt_lambertian_new_with_tex(texture_t *tex) {
    lambertian_t    *material = calloc(1, sizeof(lambertian_t));
    if (material == NULL)
        return (NULL);
    material->texture = tex;
    material_base_innit(&material->base, MATERIAL_TYPE_DIFFUSE_LAMBERTIAN, mt_lambertian_scatter, NULL, mt_lambertian_delete);

    return (material_t *)material;
}

material_t      *mt_lambertian_new_with_colour(color_t colour) {
    return mt_lambertian_new_with_tex(solid_colour_new_with_components(colour.x, colour.y, colour.z));
}

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    (void)ray_in;

    lambertian_t    *diffuse = (lambertian_t *)material;

    vec3_t scatter_dir = vec3_sum(rec->normal, vec3_random_unit_vec());
    if (vec3_near_zero(&scatter_dir))
        scatter_dir = rec->normal;
    *scattered = ray(rec->p, scatter_dir, ray_in->time);
    *attenuation = texture_t_get_value(diffuse->texture, rec->u, rec->v, &rec->p);
    return (true);
}

static void     mt_lambertian_delete(material_t *material) {
    if (material == NULL)
        return ;
    if (material->type != MATERIAL_TYPE_DIFFUSE_LAMBERTIAN) {
        printf("wrong type, material haven't been freed properly");
        return ;
    }
    lambertian_t    *diffuse = (lambertian_t *)material;
    texture_t_delete(diffuse->texture);
    free(diffuse);
}

#endif