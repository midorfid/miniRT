#include "../include/pdfs/pdf_shared.h"

void        pdf_innit(pdf_t *pdf, pdf_type_t type, pdf_delete_fn delete_pdf, pdf_value_fn get_value, pdf_generate_fn generate) {
    pdf->type = type;
    pdf->delete_pdf = delete_pdf;
    pdf->get_value = get_value;
    pdf->generate = generate;
}
