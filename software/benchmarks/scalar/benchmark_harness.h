#ifndef BENCHMARK_HARNESS_H
#define BENCHMARK_HARNESS_H

#include <stddef.h>
#include <time.h>

#define BENCH_NUM_SIZES 5

extern const size_t BENCH_SIZES[BENCH_NUM_SIZES]; /* {8, 16, 32, 64, 128} */

typedef struct {
    double elapsed_s;  /* wall-clock seconds */
    double elapsed_us; /* same, in microseconds */
    double gflops;     /* GFLOP/s; 0.0 if elapsed < 1 clock tick */
} bench_result_t;

double compute_gflops(size_t m, size_t k, size_t n, double elapsed_s);
double clocks_to_us(clock_t ticks);
bench_result_t bench_gemm_f32(size_t m, size_t k, size_t n);
bench_result_t bench_gemm_i8(size_t m, size_t k, size_t n);

#endif /* BENCHMARK_HARNESS_H */
