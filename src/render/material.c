#include "../../include/render/material_shared.h"
#include "../../include/render/material.h"

bool    material_scatter(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered, double *pdf_value) {
    if (material == NULL) {
        printf("cannot proceed, material is unknown");
        return (false);
    }
    return material->scatter(material, ray_in, rec, attenuation, scattered);
}

double    material_scatter_pdf(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered) {
    if (material == NULL) {
        printf("cannot proceed, material is unknown");
        return (false);
    }
    return material->scatter_pdf(material, ray_in, rec, scattered);
}

static bool scatter_base(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, color_t *attenuation, ray_t *scattered);

static color_t emmit_base(const material_t *material, double u, double v, const point3_t *p);

static void delete_base(material_t *material);

static double scattering_pdf_base(const material_t *material, const ray_t *ray_in, const hit_record_t *rec, const ray_t *scattered);

void    material_base_innit(material_t *material_base, material_type_t type, material_scatter_met scatter_met, 
                            material_emmit_met emmit_met, material_delete_met delete_met, material_scatter_pdf_met scattering_pdf_met) {
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
    if (scattering_pdf_met == NULL)
        scattering_pdf_met = scattering_pdf_base;
    material_base->scatter_pdf = scattering_pdf_met;    
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

static double scattering_pdf_base(const material_t *material, const ray_t *ray_in, const hit_record *rec, const ray_t *scattered) {
    return 0;
}



void    material_delete(material_t *material) {
    if (material == NULL) {
        printf("material_delete() failed");
        return ;
    }
    return material->delete(material);
}

color_t     material_emmit(const material_t *material, double u, double v, const point3_t *point) {
    if (material == NULL) {
        printf("material_emmit() failed");
        return color_in(0, 0, 0);
    }
    
    return material->emmit(material, u, v, point);
}

material_t      *material_claim(material_t *material) {
    if (material == NULL) {
        printf("material_claim() failed");
        return NULL;
    }
    material->refcount++;
    return material;
}
