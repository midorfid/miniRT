#ifndef MATERIAL_SHARED_H
#define MATERIAL_SHARED_H

#include "ray.h"
#include "hit_record.h"
#include "color.h"

typedef enum material_type_e
{
    MATERIAL_TYPE_UNKNOWN = 0,
    MATERIAL_TYPE_DIFFUSE_LAMBERTIAN = 1,
} material_type_t;


typedef bool (*material_scatter_met)(const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

struct material_s {
    material_type_t     type;
    int                 refcount;

    material_scatter_met    scatter;
    material_emmit_met      emmit;
    material_delete_met     delete;
};

void    material_base_innit(material_t *material_base, material_type_t type, material_scatter_met scatter, material_emmit_met emmit, material_delete_met delete);

#endif