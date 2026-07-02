#include <stdio.h>

#include "benchmark_harness.h"

int main(void) {
    printf("%-12s %-8s %-14s %-14s %-12s\n", "kernel", "N", "elapsed_us", "elapsed_s", "GFLOP/s");
    printf("%-12s %-8s %-14s %-14s %-12s\n", "------------", "--------", "--------------",
           "--------------", "------------");

    for (int i = 0; i < BENCH_NUM_SIZES; i++) {
        size_t n = bench_sizes[i];
        bench_result_t r = bench_gemm_f32(n, n, n);
        if (r.elapsed_s > 0.0)
            printf("gemm_f32    %-8zu %-14.2f %-14.6f %-12.4f\n", n, r.elapsed_us, r.elapsed_s,
                   r.gflops);
        else
            printf("gemm_f32    %-8zu %-14s %-14s %-12s\n", n, "<1 tick", "<1 tick", "n/a");
    }

    printf("\n");

    for (int i = 0; i < BENCH_NUM_SIZES; i++) {
        size_t n = bench_sizes[i];
        bench_result_t r = bench_gemm_i8(n, n, n);
        if (r.elapsed_s > 0.0)
            printf("gemm_i8     %-8zu %-14.2f %-14.6f %-12.4f\n", n, r.elapsed_us, r.elapsed_s,
                   r.gflops);
        else
            printf("gemm_i8     %-8zu %-14s %-14s %-12s\n", n, "<1 tick", "<1 tick", "n/a");
    }

    return 0;
}
