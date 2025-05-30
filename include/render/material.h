#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct material_s material_t;

bool    material_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

color_t     material_emmit(const material_t *material, double u, double v, const point3_t *point);

void    material_delete(material_t *material);

#endif