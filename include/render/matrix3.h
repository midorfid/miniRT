#ifndef MATRIX3_H
#define MATRIX3_H

#include "vec3.h"

typedef union matrix3_s {
    double  matrix[3][3];
    vec3_t  rows[3];
    double  matrix_flat[9];
} matrix3_t;


void            matrix_default(matrix3_t *matrix);

matrix3_t        matrix_rotation_y(double angle);

matrix3_t        matrix_scale(vec3_t scale_v);

void             matrix_multi_by_matrix(matrix3_t *a, const matrix3_t b);

vec3_t              vec3_multi_by_matrix(const vec3_t *a, const matrix3_t *matrix);

matrix3_t           matrix_inverse_scale(const matrix3_t *src);

#endif