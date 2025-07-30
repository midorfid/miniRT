#ifndef PDF_COSINE_H
#define PDF_COSINE_H

#include "pdf_shared.h"
#include "../ONB.h"

typedef struct pdf_cosine_s {
    pdf_t   base;

    onb_t   uvw;
} pdf_cosine_t;

static inline vec3_t                  cosine_pdf_generate(const pdf_t *pdf) {
    pdf_cosine_t *cos_pdf = (pdf_cosine_t *)pdf;

    vec3_t temp = random_cosine_direction();

    return onb_transform(&temp, &cos_pdf->uvw);
}

static inline double                  cosine_pdf_get_value(const pdf_t *pdf, const vec3_t *dir) {
    pdf_cosine_t *cos_pdf = (pdf_cosine_t *)pdf;

    double cos_theta = vec3_dot(cos_pdf->uvw.axis[W], *dir);
    return fmax(0.0, cos_theta / PI);

}

static inline void                    cosine_pdf_delete(pdf_t *pdf) {
    free(pdf);
}

static inline pdf_t                   *cosine_pdf_new(const vec3_t *w) {
    pdf_cosine_t    *new_pdf = calloc(1, sizeof(pdf_cosine_t));
    if (new_pdf == NULL) {
        printf("cosine_pdf_new failed");
        return NULL;
    }
    new_pdf->uvw = onb_init(w);

    pdf_innit(&new_pdf->base, PDF_COSINE, cosine_pdf_delete, cosine_pdf_get_value, cosine_pdf_generate);

    return (pdf_t *)new_pdf;
}

#endif