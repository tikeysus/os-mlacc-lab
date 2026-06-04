#include <stdio.h>

#include "../../include/ml_kernels.h"

int main(void) {
    const float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};

    const float result = dot_product_f32(a, b, 4);

    printf("dot_product_f32 = %.2f\n", result);

    return 0;
}
