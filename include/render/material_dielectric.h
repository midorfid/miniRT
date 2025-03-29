#ifndef MATERIAL_DIELECTRIC_H
#define MATERIAL_DIELECTRIC_H

#include "hit_record.h"
#include "ray.h"
#include "material_shared.h"
#include "material.h"

typedef struct dielectric_s
{
    material_t  base;

    double      refraction_index;
} dielectric_t;

static bool     mt_dielectric_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void     mt_dielectric_delete(material_t *material);

material_t      *mt_dielectric_new(double refraction_index);


material_t      *mt_dielectric_new(double refraction_index) {
    dielectric_t    *material = calloc(1, sizeof(dielectric_t));
    if (material == NULL)
        return (NULL);
    material->refraction_index = refraction_index;
    material_base_innit(&material->base, MATERIAL_TYPE_DIELECTRIC, mt_dielectric_scatter, NULL, mt_dielectric_delete);

    return (material_t *)material;
}

static bool     mt_dielectric_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    (void)ray_in;

    dielectric_t    *dielectric = (dielectric_t *)material;

    *attenuation = color_in(1.0, 1.0, 1.0);
    double ri = rec->front_face ? (1.0/dielectric->refraction_index) : dielectric->refraction_index;

    vec3_t unit_direction = vec3_normalize(ray_in->dir);
    vec3_t refracted = refract(&unit_direction, &rec->normal, ri);

    *scattered = ray(rec->p, refracted);
    return (true);
}

static void     mt_dielectric_delete(material_t *material) {
    if (material == NULL)
        return ;
    if (material->type != MATERIAL_TYPE_DIELECTRIC) {
        printf("wrong type, material haven't been freed properly");
        return ;
    }
    dielectric_t    *dielectric = (dielectric_t *)material;
    free(dielectric);
}

#endif