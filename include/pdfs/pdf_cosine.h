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
    vec3_t res = onb_transform(&temp, &cos_pdf->uvw);
    // printf("random_cos: %f %f %f\n", res.x, res.y, res.z);
    // fflush(stdout);

    return res;
}

static inline double                  cosine_pdf_get_value(const pdf_t *pdf, const vec3_t *dir) {
    pdf_cosine_t *cos_pdf = (pdf_cosine_t *)pdf;

    // printf("W: %f %f %f\n dir: %f %f %f\n", cos_pdf->uvw.axis[W].x, cos_pdf->uvw.axis[W].y, cos_pdf->uvw.axis[W].z,
        // vec3_normalize(*dir).x, vec3_normalize(*dir).y, vec3_normalize(*dir).z);
    // fflush(stdout);
    double cos_theta = vec3_dot(cos_pdf->uvw.axis[W], vec3_normalize(*dir));
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