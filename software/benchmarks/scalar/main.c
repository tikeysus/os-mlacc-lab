#include <stdio.h>
#include <time.h>

#include "ml_kernels.h"

#define GEMM_N 64

int main(void) {
    const float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};

    printf("dot_product_f32 = %.2f\n", dot_product_f32(a, b, 4));

    static float mat_a[GEMM_N * GEMM_N];
    static float mat_b[GEMM_N * GEMM_N];
    static float mat_c[GEMM_N * GEMM_N];

    for (int i = 0; i < GEMM_N * GEMM_N; i++) {
        mat_a[i] = (float)(i % 7 - 3);
        mat_b[i] = (float)(i % 5 - 2);
    }

    clock_t t0 = clock();
    gemm_f32(mat_a, mat_b, mat_c, GEMM_N, GEMM_N, GEMM_N);
    clock_t t1 = clock();

    double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

    if (elapsed > 0.0) {
        double gflops = 2.0 * GEMM_N * GEMM_N * GEMM_N / (elapsed * 1e9);
        printf("gemm_f32 %dx%d: %.6f s, %.3f GFLOP/s\n", GEMM_N, GEMM_N, elapsed, gflops);
    } else {
        printf("gemm_f32 %dx%d: <1 clock tick (matrix too small to time)\n", GEMM_N, GEMM_N);
    }

    return 0;
}
