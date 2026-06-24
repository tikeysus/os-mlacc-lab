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

/* a is m×k, b is k×n, c is m×n — all row-major */
void gemm_f32(const float *a, const float *b, float *c, size_t m, size_t k, size_t n);
void gemm_i8(const int8_t *a, const int8_t *b, int32_t *c, size_t m, size_t k, size_t n);

#endif
