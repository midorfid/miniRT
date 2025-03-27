#ifndef MATERIAL_METAL_H
#define MATERIAL_METAL_H

#include "hit_record.h"
#include "ray.h"
#include "material_shared.h"
#include "material.h"

typedef struct metal_s
{
    material_t  base;
    
    color_t     albedo;
    double      fuzz;
} metal_t;

static bool     mt_metal_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void     mt_metal_delete(material_t *material);

material_t      *mt_metal_new(color_t albedo, double fuzz) {
    metal_t    *material = calloc(1, sizeof(metal_t));
    if (material == NULL)
        return (NULL);
    material->albedo = albedo;
    material->fuzz = fuzz < 1 ? fuzz : 1;
    material_base_innit(&material->base, MATERIAL_TYPE_METAL, mt_metal_scatter, NULL, mt_metal_delete);

    return (material_t *)material;
}

static bool     mt_metal_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    // (void)ray_in;

    metal_t    *diffuse = (metal_t *)material;

    vec3_t reflected = mirrored_reflect(&ray_in->dir, &rec->normal);
    reflected = vec3_sum(vec3_normalize(reflected), vec3_scaled_return(vec3_random_unit_vec(), diffuse->fuzz));
    *scattered = ray(rec->p, reflected);
    *attenuation = diffuse->albedo;

    return (vec3_dot(scattered->dir, rec->normal) > 0);
}

static void     mt_metal_delete(material_t *material) {
    if (material == NULL)
        return ;
    if (material->type != MATERIAL_TYPE_METAL) {
        printf("wrong type, material haven't been freed properly");
        return ;
    }
    metal_t    *diffuse = (metal_t *)material;
    free(diffuse);
}

#endif