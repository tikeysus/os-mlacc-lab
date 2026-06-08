#ifndef ML_KERNELS_H
#define ML_KERNELS_H

#include <stddef.h>
#include <stdint.h>

typedef struct ml_matrix_shape {
    size_t rows;
    size_t cols;
} ml_matrix_shape;

float dot_product_f32(const float *a, const float *b, size_t n);
int32_t dot_product_i8(const int8_t *a, const int8_t *b, size_t n);
void gemv_f32(const float *matrix, const float *vector, float *output, ml_matrix_shape shape);

#endif
