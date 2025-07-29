#ifndef MATERIAL_DIFFUSE_LIGHT_H
#define MATERIAL_DIFFUSE_LIGHT_H

#include "../../include/render/hit_record.h"
#include "../../include/render/ray.h"
#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"

typedef struct diffuse_light_s {
    material_t  base;

    texture_t   *texture;
    // double      intensity;
} diffuse_light_t;

bool    diffuse_light_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

color_t    diffuse_light_emmit(const material_t *material, double u, double v, const point3_t *point, const hit_record_t *rec);

void    diffuse_light_delete(material_t *material);

material_t      *diffuse_light_new_with_tex(texture_t *texture);

material_t      *diffuse_light_new_with_colour(color_t albedo);

#endif