#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"

bool    material_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    if (material == NULL) {
        printf("cannot proceed, material is unknown");
        return (false);
    }
    return material->scatter(material, ray_in, rec, attenuation, scattered);
}

static bool scatter_base(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static color_t emmit_base(const material_t *material, double u, double v, const point3_t *p);

static void delete_base(material_t *material);

void    material_base_innit(material_t *material_base, material_type_t type, material_scatter_met scatter_met, material_emmit_met emmit_met, material_delete_met delete_met) {
    material_base->type = type;
    material_base->refcount = 1;

    if (scatter_met == NULL)
        scatter_met = scatter_base;
    material_base->scatter = scatter_met;
    if (emmit_met == NULL)
        emmit_met = emmit_base;
    material_base->emmit = emmit_met;
    if (delete_met == NULL)
        delete_met = delete_base;
    material_base->delete = delete_met;
}

static bool scatter_base(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered) {
    return (false);
}

static color_t emmit_base(const material_t *material, double u, double v, const point3_t *p) {
    return (color_in(0.0, 0.0, 0.0));
}

static void delete_base(material_t *material) {
    free(material);
}