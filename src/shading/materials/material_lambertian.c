#ifndef MATERIAL_LAMBERTIAN_H
#define MATERIAL_LAMBERTIAN_H

#include "../../include/render/hit_record.h"
#include "../../include/render/ray.h"
#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"
#include "../../include/textures/solid_colour.h"
#include "../../include/ONB.h"
#include "../../include/pdfs/pdf_cosine.h"

typedef struct lambertian_s
{
    material_t  base;
    
    texture_t   *texture;
} lambertian_t;

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, scatter_record_t *srec);

static void     mt_lambertian_delete(material_t *material);

static double   mt_lambertian_scatter_pdf(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered);

material_t      *mt_lambertian_new_with_tex(texture_t *tex) {
    lambertian_t    *material = calloc(1, sizeof(lambertian_t));
    if (material == NULL)
        return (NULL);
    material->texture = tex;
    material_base_innit(&material->base, MATERIAL_TYPE_DIFFUSE_LAMBERTIAN, mt_lambertian_scatter, NULL, mt_lambertian_delete, mt_lambertian_scatter_pdf);

    return (material_t *)material;
}

material_t      *mt_lambertian_new_with_colour(color_t colour) {
    return mt_lambertian_new_with_tex(solid_colour_new_with_components(colour.x, colour.y, colour.z));
}

static bool     mt_lambertian_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, scatter_record_t *srec) {
    onb_t   uvw;
    
    lambertian_t    *diffuse = (lambertian_t *)material;

    srec->attenuation = diffuse->texture->get_value(diffuse->texture, rec->u, rec->v, &rec->p);
    srec->pdf_ptr = cosine_pdf_new(&rec->normal);
    srec->skip_pdf = false;

    // uvw = onb_init(&rec->normal);
    
    // vec3_t random_cos_dir = random_cosine_direction();
    // vec3_t scatter_dir = onb_transform(&random_cos_dir, &uvw);

    // *scattered = ray(rec->p, vec3_normalize(scatter_dir), ray_in->time);
    // *attenuation = texture_t_get_value(diffuse->texture, rec->u, rec->v, &rec->p);

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

static double mt_lambertian_scatter_pdf(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered) {
    if (material == NULL || material->type != MATERIAL_TYPE_DIFFUSE_LAMBERTIAN) {
        printf("wrong type, scatter_pdf mat failed");
        return 0.0;
    }
    
    double cos_theta = vec3_dot(rec->normal, vec3_normalize(scattered->dir));
    return cos_theta < 0.0 ? 0.0 : cos_theta / PI;
}

#endif