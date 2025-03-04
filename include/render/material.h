#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct material_s material_t;

bool    material_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);


#endif