#ifndef MATERIAL_LAMBERTIAN_H
#define MATERIAL_LAMBERTIAN_H

#include "hit_record.h"
#include "ray.h"
#include "material_shared.h"
#include "material.h"

typedef struct lambertian_s
{
    material_t  base;
    
    color_t     albedo;
} lambertian_t;

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void     mt_lambertian_delete(material_t *material);

material_t      *mt_lambertian_new(color_t albedo) {
    lambertian_t    *material = calloc(1, sizeof(lambertian_t));
    if (material == NULL)
        return (NULL);
    material->albedo = albedo;
    material_base_innit(&material->base, MATERIAL_TYPE_DIFFUSE_LAMBERTIAN, mt_lambertian_scatter, NULL, mt_lambertian_delete);

    return (material_t *)material;
}

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    (void)ray_in;

    lambertian_t    *diffuse = (lambertian_t *)material;

    vec3_t scatter_dir = vec3_sum(rec->normal, vec3_random_unit_vec());
    if (vec3_near_zero(&scatter_dir))
        scatter_dir = rec->normal;
    *scattered = ray(rec->p, scatter_dir, ray_in->time);
    *attenuation = diffuse->albedo;

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
    free(diffuse);
}

#endif