#ifndef PDF_HITTABLE_H
#define PDF_HITTABLE_H

#include "pdf_shared.h"
#include "../render/hittable_list.h"

typedef struct pdf_hittable_s {
    pdf_t                       base;

    point3_t                    origin;
    const hittable_list_t     *objects;
} pdf_hittable_t;

static inline vec3_t                  hittable_pdf_generate(const pdf_t *pdf) {
    pdf_hittable_t *hittable_pdf = (pdf_hittable_t *)pdf;

    int upper_bound = (int)hittable_pdf->objects->size;
    int random_hittable = my_random_int(0, upper_bound - 1);

    hittable_t *chosen = hittable_pdf->objects->hittables[random_hittable];

    return chosen->random(chosen, &hittable_pdf->origin);
}

static inline double                  hittable_pdf_get_value(const pdf_t *pdf, const vec3_t *dir) {
    pdf_hittable_t *hittable_pdf = (pdf_hittable_t *)pdf;
    
    int lights_count = (int)hittable_pdf->objects->size;
    double sum = 0.0;
    for (int i = 0; i < lights_count; ++i) {
        hittable_t *chosen = hittable_pdf->objects->hittables[i];
        sum += chosen->pdf_value(chosen, &hittable_pdf->origin, dir);
    }

    return sum;
}

static inline void                    hittable_pdf_delete(pdf_t *pdf) {
    pdf_hittable_t *hittable_pdf = (pdf_hittable_t *)pdf;
    
    free(pdf);
}

static inline pdf_t                   *hittable_pdf_new(const hittable_list_t *objects, const point3_t *p) {
    pdf_hittable_t    *new_pdf = calloc(1, sizeof(pdf_hittable_t));
    if (new_pdf == NULL) {
        printf("hittable_pdf_new failed");
        return NULL;
    }
    new_pdf->objects = objects;
    new_pdf->origin = *p;
    
    pdf_innit(&new_pdf->base, PDF_HITTABLE, hittable_pdf_delete, hittable_pdf_get_value, hittable_pdf_generate);

    return (pdf_t *)new_pdf;
}


#endif