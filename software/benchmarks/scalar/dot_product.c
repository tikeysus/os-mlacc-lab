#include "../../include/ml_kernels.h"

float dot_product_f32(const float *a, const float *b, size_t n) {
    float sum = 0.0f;

    for (size_t i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }

    return sum;
}
