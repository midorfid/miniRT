#ifndef PDF_SHARED_H
#define PDF_SHARED_H

#include "pdf.h"

typedef void        (*pdf_delete_fn)(pdf_t *pdf);
typedef double      (*pdf_value_fn)(const pdf_t *pdf, const vec3_t *dir);
typedef vec3_t      (*pdf_generate_fn)(const pdf_t *pdf);

typedef enum pdf_type_e {
    PDF_SPHERE,
    PDF_COSINE,
    PDF_HITTABLE,
    PDF_MIXTURE,
} pdf_type_t;


struct pdf_s {
    pdf_type_t          type;

    pdf_delete_fn       delete_pdf;
    pdf_value_fn        get_value;
    pdf_generate_fn     generate;    
};

void        pdf_innit(pdf_t *pdf, pdf_type_t type, pdf_delete_fn delete_pdf, pdf_value_fn get_value, pdf_generate_fn generate);

#endif