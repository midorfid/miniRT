#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "../textures/texture_shared.h"

typedef struct material_s material_t;

bool    material_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

double    material_scatter_pdf(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered);

color_t     material_emmit(const material_t *material, double u, double v, const point3_t *point);

void    material_delete(material_t *material);

material_t      *material_claim(material_t *material);


material_t      *mt_metal_new(color_t albedo, double fuzz);

material_t      *mt_dielectric_new(double refraction_index);

material_t      *mt_lambertian_new_with_tex(texture_t *tex);

material_t      *mt_lambertian_new_with_colour(color_t colour);

material_t      *diffuse_light_new_with_tex(texture_t *texture);

material_t      *diffuse_light_new_with_colour(color_t albedo);

#endif
