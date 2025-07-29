#ifndef PDF_SHPERE_H
#define PDF_SPHERE_H

#include "pdf_shared.h"

typedef struct pdf_sphere_s {
    pdf_t   base;
} pdf_sphere_t;

vec3_t                  sphere_pdf_generate(const pdf_t *pdf) {
    return vec3_random_unit_vec();
}

double                  sphere_pdf_get_value(const pdf_t *pdf, const vec3_t *dir) {
    return 1.0 / (4.0 * PI);
}

void                    sphere_pdf_delete(pdf_t *pdf) {
    free(pdf);
}

pdf_t                   *sphere_pdf_new() {
    pdf_sphere_t    *new_pdf = calloc(1, sizeof(pdf_sphere_t);
    if (new_pdf == NULL) {
        printf("sphere_pdf_new failed");
        return new_pdf;
    }
    pdf_innit(&new_pdf->base, PDF_SPHERE, sphere_pdf_delete, sphere_pdf_get_value, sphere_pdf_generate);

    return new_pdf;
}

#endif