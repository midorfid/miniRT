#ifndef PDF_MIXTURES_H
#define PDF_MIXTURES_H

#include "pdf_shared.h"

typedef struct mixture_pdf_s {
    pdf_t       base;

    pdf_t       *pdf[2];
} mixture_pdf_t;

static inline void        mixture_pdf_delete(pdf_t *mix_pdf) {
    if (mix_pdf == NULL) {
        printf("mixture_pdf_delete() failed.");
        return ;
    }
    mixture_pdf_t   *to_delete = (mixture_pdf_t *)mix_pdf;

    for (int i = 0; i < 2; ++i) {
        to_delete->pdf[i]->delete_pdf(to_delete->pdf[i]);
    }
    free(to_delete);
}

static inline double      mixture_pdf_get_value(const pdf_t *mix_pdf, const vec3_t *dir) {
    if (mix_pdf == NULL) {
        printf("mixture_pdf_get_value() failed.");
        return 0.0;
    }
    mixture_pdf_t   *pdfs = (mixture_pdf_t *)mix_pdf;
    
    // printf("light_pdf:%f\n", pdfs->pdf[0]->get_value(pdfs->pdf[0],dir));
    // printf("cosine_pdf:%f\n", pdfs->pdf[1]->get_value(pdfs->pdf[1],dir));
    // fflush(stdout);
    
    return 0.5 * pdfs->pdf[0]->get_value(pdfs->pdf[0], dir) + 0.5 * pdfs->pdf[1]->get_value(pdfs->pdf[1], dir); 
}

static inline vec3_t      mixture_pdf_generate(const pdf_t *mix_pdf) {
    if (mix_pdf == NULL) {
        printf("mixture_pdf_generate() failed.");
        return vec3(1,0,0);
    }
    mixture_pdf_t   *pdfs = (mixture_pdf_t *)mix_pdf;

    if (random_double_nolimits() < 0.5)
        return pdfs->pdf[0]->generate(pdfs->pdf[0]);
    else
        return pdfs->pdf[1]->generate(pdfs->pdf[1]);
}


static inline pdf_t       *mixture_pdf_new(pdf_t *p0, pdf_t *p1) {
    mixture_pdf_t       *mix_pdf = calloc(1, sizeof(mixture_pdf_t));
    if (mix_pdf == NULL) {
        printf("mixture_pdf_new() failed.");
        return NULL;
    }
    mix_pdf->pdf[0] = p0;
    mix_pdf->pdf[1] = p1;

    pdf_innit(&mix_pdf->base, PDF_MIXTURE, mixture_pdf_delete, mixture_pdf_get_value, mixture_pdf_generate);

    return (pdf_t *)mix_pdf;
}

#endif