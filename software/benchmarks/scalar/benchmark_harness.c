#include "benchmark_harness.h"
#include "ml_kernels.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const size_t bench_sizes[BENCH_NUM_SIZES] = {8, 16, 32, 64, 128};

double compute_gflops(size_t m, size_t k, size_t n, double elapsed_s) {
    if (elapsed_s <= 0.0)
        return 0.0;
    return 2.0 * (double)m * (double)k * (double)n / (elapsed_s * 1e9);
}

double clocks_to_us(clock_t ticks) { return (double)ticks * 1e6 / (double)CLOCKS_PER_SEC; }

bench_result_t bench_gemm_f32(size_t m, size_t k, size_t n) {
    float *a = (float *)calloc(m * k, sizeof(float));
    float *b = (float *)calloc(k * n, sizeof(float));
    float *c = (float *)calloc(m * n, sizeof(float));
    if (!a || !b || !c) {
        fputs("bench_gemm_f32: calloc failed\n", stderr);
        free(a);
        free(b);
        free(c);
        abort();
    }

    for (size_t i = 0; i < m * k; i++)
        a[i] = (float)(i + 1);
    for (size_t i = 0; i < k * n; i++)
        b[i] = (float)(i + 1);

    clock_t t0 = clock();
    gemm_f32(a, b, c, m, k, n);
    clock_t t1 = clock();

    clock_t ticks = t1 - t0;
    double elapsed_s = (double)ticks / (double)CLOCKS_PER_SEC;

    bench_result_t r;
    r.elapsed_s = elapsed_s;
    r.elapsed_us = clocks_to_us(ticks);
    r.gflops = compute_gflops(m, k, n, elapsed_s);

    free(a); /* freed locally */
    free(b);
    free(c);
    return r;
}

bench_result_t bench_gemm_i8(size_t m, size_t k, size_t n) {
    int8_t *a = (int8_t *)calloc(m * k, sizeof(int8_t));
    int8_t *b = (int8_t *)calloc(k * n, sizeof(int8_t));
    int32_t *c = (int32_t *)calloc(m * n, sizeof(int32_t));
    if (!a || !b || !c) {
        fputs("bench_gemm_i8: calloc failed\n", stderr);
        free(a);
        free(b);
        free(c);
        abort();
    }

    for (size_t i = 0; i < m * k; i++)
        a[i] = (int8_t)((i % 100) + 1);
    for (size_t i = 0; i < k * n; i++)
        b[i] = (int8_t)((i % 100) + 1);

    clock_t t0 = clock();
    gemm_i8(a, b, c, m, k, n);
    clock_t t1 = clock();

    clock_t ticks = t1 - t0;
    double elapsed_s = (double)ticks / (double)CLOCKS_PER_SEC;

    bench_result_t r;
    r.elapsed_s = elapsed_s;
    r.elapsed_us = clocks_to_us(ticks);
    r.gflops = compute_gflops(m, k, n, elapsed_s);

    free(a); /* freed locally */
    free(b);
    free(c);
    return r;
}
