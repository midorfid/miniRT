#ifndef MATERIAL_DIELECTRIC_H
#define MATERIAL_DIELECTRIC_H

#include "../../include/render/hit_record.h"
#include "../../include/render/ray.h"
#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"

typedef struct dielectric_s
{
    material_t  base;

    double      refraction_index;
} dielectric_t;

static bool     mt_dielectric_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void     mt_dielectric_delete(material_t *material);

material_t      *mt_dielectric_new(double refraction_index) {
    dielectric_t    *material = calloc(1, sizeof(dielectric_t));
    if (material == NULL)
        return (NULL);
    material->refraction_index = refraction_index;
    material_base_innit(&material->base, MATERIAL_TYPE_DIELECTRIC, mt_dielectric_scatter, NULL, mt_dielectric_delete, NULL);

    return (material_t *)material;
}
// Schlick's approximation for reflectance
static double reflectance(double cosine, double refraction_index) {
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return (r0 + (1-r0)*pow((1 - cosine),5));
}

static bool     mt_dielectric_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered, double *pdf_value) {
    (void)ray_in;

    dielectric_t    *dielectric = (dielectric_t *)material;

    *attenuation = color_in(1.0, 1.0, 1.0);
    double ri = rec->front_face ? (1.0/dielectric->refraction_index) : dielectric->refraction_index;

    vec3_t unit_direction = vec3_normalize(ray_in->dir);
    double cos_theta = fmin(vec3_dot(vec3_negative(&unit_direction), rec->normal), 1.0);
    double sin_theta = sqrt(1 - cos_theta*cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3_t  direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double_nolimits()) {
        direction = reflect(&ray_in->dir, &rec->normal);
    }
    else {
        vec3_t direction = refract(&unit_direction, &rec->normal, ri); 
    }

    *scattered = ray(rec->p, direction, ray_in->time);
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