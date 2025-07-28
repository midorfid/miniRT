#ifndef MATERIAL_ISOTROPIC_H
#define MATERIAL_ISOTROPIC_H

#include "material_shared.h"
#include "../textures/texture_shared.h"
#include "../textures/solid_colour.h"

typedef struct isotropic_s {
    material_t  base;

    texture_t   *albedo;
} isotropic_t;

static bool         isotropic_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static void         isotropic_delete(material_t *mat);

static double       iso_scatter_pdf(const material_t *mat, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered);

material_t          *isotropic_new_with_tex(texture_t *tex);

material_t          *isotropic_new_with_colour(color_t albedo);

#endif