#include "ml_kernels.h"

void gemm_f32(const float *a, const float *b, float *c, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            float acc = 0.0f;
            for (size_t p = 0; p < k; p++)
                acc += a[i * k + p] * b[p * n + j];
            c[i * n + j] = acc;
        }
    }
}

void gemm_i8(const int8_t *a, const int8_t *b, int32_t *c, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            int32_t acc = 0;
            for (size_t p = 0; p < k; p++)
                acc += (int32_t)a[i * k + p] * (int32_t)b[p * n + j];
            c[i * n + j] = acc;
        }
    }
}
