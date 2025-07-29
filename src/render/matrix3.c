#include "../../include/render/matrix3.h"

matrix3_t     matrix_rotation_y(double angle) {
    double      theta_cos = cos(angle);
    double      theta_sin = sin(angle);
    // cos(A+B)=cosAcosB−sinAsinB
    // sin(A+B)=sinAcosB+cosAsinB
    matrix3_t result = { .matrix =
    {
        theta_cos, 0, theta_sin,
        0, 1, 0,
        -theta_sin, 0, theta_cos,
    }};

    return result;
}

matrix3_t    matrix_scale(vec3_t scale_v) {
    matrix3_t result = { .matrix =
    {
        scale_v.x, 0, 0,
        0, scale_v.y, 0,
        0, 0, scale_v.z,
    }};

    return result;
}

void        matrix_multi_by_matrix(matrix3_t *first, const matrix3_t second) {
    matrix3_t result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.matrix[i][j] = first->rows[i].components[0] * second.matrix[0][j] +
                        first->rows[i].components[1] * second.matrix[1][j] +
                        first->rows[i].components[2] * second.matrix[2][j];
        }
    }
    *first = result;
}

//brittle
matrix3_t           matrix_inverse_scale(const matrix3_t *src) {
    matrix3_t       inversed = { .matrix =
        {
            1.0 / src->matrix_flat[0], 0, 0,
            0, 1.0 / src->matrix_flat[4], 0,
            0, 0, 1.0 / src->matrix_flat[8],
        }};
    return inversed;
}
