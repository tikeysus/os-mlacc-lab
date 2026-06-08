#include "ml_kernels.h"

int32_t dot_product_i8(const int8_t *a, const int8_t *b, size_t n) {
    int32_t sum = 0;

    for (size_t i = 0; i < n; i++) {
        sum += (int32_t)a[i] * (int32_t)b[i];
    }

    return sum;
}
