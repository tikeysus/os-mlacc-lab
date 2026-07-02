#include "unity.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "benchmark_harness.h"
#include "ml_kernels.h"

void set_up(void) {}

void tear_down(void) {}

/* -------------------------------------------------------------------------
 * Group 1: compute_gflops() formula correctness
 * ------------------------------------------------------------------------- */

static void test_compute_gflops_1x1x1_at_1s(void) {
    /* 2 * 1 * 1 * 1 FLOPs / (1.0 s * 1e9) = 2e-9 GFLOP/s */
    double result = compute_gflops(1, 1, 1, 1.0);
    TEST_ASSERT_DOUBLE_WITHIN(1e-18, 2e-9, result);
}

static void test_compute_gflops_64x64x64(void) {
    /* 2 * 64^3 = 524288 FLOPs; at 1.0 s → 5.24288e-4 GFLOP/s */
    double expected = 2.0 * 64.0 * 64.0 * 64.0 / 1e9;
    double result = compute_gflops(64, 64, 64, 1.0);
    TEST_ASSERT_DOUBLE_WITHIN(expected * 1e-9, expected, result);
}

static void test_compute_gflops_doubles_when_elapsed_halved(void) {
    double g1 = compute_gflops(32, 32, 32, 1.0);
    double g2 = compute_gflops(32, 32, 32, 0.5);
    TEST_ASSERT_DOUBLE_WITHIN(g2 * 1e-9, g1 * 2.0, g2);
}

static void test_compute_gflops_zero_elapsed_returns_zero(void) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, compute_gflops(64, 64, 64, 0.0));
    TEST_ASSERT_EQUAL_DOUBLE(0.0, compute_gflops(64, 64, 64, -1.0));
}

/* -------------------------------------------------------------------------
 * Group 2: clocks_to_us() conversion
 * ------------------------------------------------------------------------- */

static void test_clocks_to_us_zero_ticks(void) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, clocks_to_us(0));
}

static void test_clocks_to_us_one_full_second(void) {
    double result = clocks_to_us((clock_t)CLOCKS_PER_SEC);
    TEST_ASSERT_DOUBLE_WITHIN(0.5, 1000000.0, result);
}

static void test_clocks_to_us_half_second(void) {
    double result = clocks_to_us((clock_t)(CLOCKS_PER_SEC / 2));
    TEST_ASSERT_DOUBLE_WITHIN(1.0, 500000.0, result);
}

static void test_clocks_to_us_result_unit_is_microseconds(void) {
    /* One tick should be 1e6 / CLOCKS_PER_SEC microseconds */
    double expected = 1e6 / (double)CLOCKS_PER_SEC;
    double result = clocks_to_us(1);
    TEST_ASSERT_DOUBLE_WITHIN(expected * 1e-9, expected, result);
}

/* -------------------------------------------------------------------------
 * Group 3: Sweep size array coverage
 * ------------------------------------------------------------------------- */

static void test_bench_sweep_size_count_is_five(void) {
    TEST_ASSERT_EQUAL_size_t(5, (size_t)BENCH_NUM_SIZES);
}

static int sizes_contains(size_t val) {
    for (int i = 0; i < BENCH_NUM_SIZES; i++) {
        if (BENCH_SIZES[i] == val)
            return 1;
    }
    return 0;
}

static void test_bench_sweep_contains_size_8(void) {
    TEST_ASSERT_TRUE(sizes_contains(8));
}

static void test_bench_sweep_contains_size_16(void) {
    TEST_ASSERT_TRUE(sizes_contains(16));
}

static void test_bench_sweep_contains_size_32(void) {
    TEST_ASSERT_TRUE(sizes_contains(32));
}

static void test_bench_sweep_contains_size_64(void) {
    TEST_ASSERT_TRUE(sizes_contains(64));
}

static void test_bench_sweep_contains_size_128(void) {
    TEST_ASSERT_TRUE(sizes_contains(128));
}

static void test_bench_sweep_sizes_are_sorted_ascending(void) {
    for (int i = 0; i < BENCH_NUM_SIZES - 1; i++)
        TEST_ASSERT_LESS_THAN_size_t(BENCH_SIZES[i + 1], BENCH_SIZES[i]);
}

/* -------------------------------------------------------------------------
 * Group 4: gemm_f32 correctness at each benchmark size
 *
 * Strategy: A = identity, B = arbitrary row-major matrix.
 * C = I * B = B, so spot-check first row and main diagonal of C against B.
 * ------------------------------------------------------------------------- */

static void fill_identity_f32(float *a, size_t n) {
    for (size_t i = 0; i < n * n; i++)
        a[i] = 0.0f;
    for (size_t i = 0; i < n; i++)
        a[i * n + i] = 1.0f;
}

static void fill_pattern_f32(float *b, size_t n) {
    for (size_t i = 0; i < n * n; i++)
        b[i] = (float)(i % 13 - 6);
}

static void check_gemm_f32_identity_at_n(size_t n) {
    float *a = (float *)calloc(n * n, sizeof(float));
    float *b = (float *)calloc(n * n, sizeof(float));
    float *c = (float *)calloc(n * n, sizeof(float));

    fill_identity_f32(a, n);
    fill_pattern_f32(b, n);

    gemm_f32(a, b, c, n, n, n);

    /* First row of C must equal first row of B */
    for (size_t j = 0; j < n; j++)
        TEST_ASSERT_EQUAL_FLOAT(b[j], c[j]);

    /* Main diagonal of C must equal main diagonal of B */
    for (size_t i = 0; i < n; i++)
        TEST_ASSERT_EQUAL_FLOAT(b[i * n + i], c[i * n + i]);

    free(a);
    free(b);
    free(c);
}

static void test_gemm_f32_correct_at_benchmark_size_8(void) {
    check_gemm_f32_identity_at_n(8);
}

static void test_gemm_f32_correct_at_benchmark_size_16(void) {
    check_gemm_f32_identity_at_n(16);
}

static void test_gemm_f32_correct_at_benchmark_size_32(void) {
    check_gemm_f32_identity_at_n(32);
}

static void test_gemm_f32_correct_at_benchmark_size_64(void) {
    check_gemm_f32_identity_at_n(64);
}

static void test_gemm_f32_correct_at_benchmark_size_128(void) {
    check_gemm_f32_identity_at_n(128);
}

/* -------------------------------------------------------------------------
 * Group 5: gemm_i8 correctness at each benchmark size
 * Same identity strategy; accumulates into int32_t.
 * ------------------------------------------------------------------------- */

static void fill_identity_i8(int8_t *a, size_t n) {
    for (size_t i = 0; i < n * n; i++)
        a[i] = 0;
    for (size_t i = 0; i < n; i++)
        a[i * n + i] = 1;
}

static void fill_pattern_i8(int8_t *b, size_t n) {
    for (size_t i = 0; i < n * n; i++)
        b[i] = (int8_t)(i % 11 - 5);
}

static void check_gemm_i8_identity_at_n(size_t n) {
    int8_t  *a = (int8_t  *)calloc(n * n, sizeof(int8_t));
    int8_t  *b = (int8_t  *)calloc(n * n, sizeof(int8_t));
    int32_t *c = (int32_t *)calloc(n * n, sizeof(int32_t));

    fill_identity_i8(a, n);
    fill_pattern_i8(b, n);

    gemm_i8(a, b, c, n, n, n);

    /* First row of C must equal first row of B (widened to int32) */
    for (size_t j = 0; j < n; j++)
        TEST_ASSERT_EQUAL_INT32((int32_t)b[j], c[j]);

    /* Main diagonal of C must equal main diagonal of B */
    for (size_t i = 0; i < n; i++)
        TEST_ASSERT_EQUAL_INT32((int32_t)b[i * n + i], c[i * n + i]);

    free(a);
    free(b);
    free(c);
}

static void test_gemm_i8_correct_at_benchmark_size_8(void) {
    check_gemm_i8_identity_at_n(8);
}

static void test_gemm_i8_correct_at_benchmark_size_16(void) {
    check_gemm_i8_identity_at_n(16);
}

static void test_gemm_i8_correct_at_benchmark_size_32(void) {
    check_gemm_i8_identity_at_n(32);
}

static void test_gemm_i8_correct_at_benchmark_size_64(void) {
    check_gemm_i8_identity_at_n(64);
}

static void test_gemm_i8_correct_at_benchmark_size_128(void) {
    check_gemm_i8_identity_at_n(128);
}

/* -------------------------------------------------------------------------
 * Group 6: bench_gemm_f32() result sanity
 * ------------------------------------------------------------------------- */

static void test_bench_gemm_f32_elapsed_s_nonnegative_at_8x8(void) {
    bench_result_t r = bench_gemm_f32(8, 8, 8);
    TEST_ASSERT_GREATER_OR_EQUAL_DOUBLE(0.0, r.elapsed_s);
}

static void test_bench_gemm_f32_elapsed_s_positive_at_128x128(void) {
    bench_result_t r = bench_gemm_f32(128, 128, 128);
    TEST_ASSERT_GREATER_THAN_DOUBLE(0.0, r.elapsed_s);
}

static void test_bench_gemm_f32_elapsed_us_matches_elapsed_s(void) {
    bench_result_t r = bench_gemm_f32(64, 64, 64);
    double expected_us = r.elapsed_s * 1e6;
    TEST_ASSERT_DOUBLE_WITHIN(expected_us * 1e-6 + 1e-3, expected_us, r.elapsed_us);
}

static void test_bench_gemm_f32_gflops_nonnegative_at_128x128(void) {
    bench_result_t r = bench_gemm_f32(128, 128, 128);
    TEST_ASSERT_GREATER_OR_EQUAL_DOUBLE(0.0, r.gflops);
}

static void test_bench_gemm_f32_gflops_consistent_with_formula(void) {
    bench_result_t r = bench_gemm_f32(128, 128, 128);
    double expected = compute_gflops(128, 128, 128, r.elapsed_s);
    TEST_ASSERT_DOUBLE_WITHIN(expected * 1e-6 + 1e-12, expected, r.gflops);
}

/* -------------------------------------------------------------------------
 * Group 7: bench_gemm_i8() result sanity
 * ------------------------------------------------------------------------- */

static void test_bench_gemm_i8_elapsed_s_nonnegative_at_8x8(void) {
    bench_result_t r = bench_gemm_i8(8, 8, 8);
    TEST_ASSERT_GREATER_OR_EQUAL_DOUBLE(0.0, r.elapsed_s);
}

static void test_bench_gemm_i8_elapsed_s_positive_at_128x128(void) {
    bench_result_t r = bench_gemm_i8(128, 128, 128);
    TEST_ASSERT_GREATER_THAN_DOUBLE(0.0, r.elapsed_s);
}

static void test_bench_gemm_i8_gflops_nonnegative_at_128x128(void) {
    bench_result_t r = bench_gemm_i8(128, 128, 128);
    TEST_ASSERT_GREATER_OR_EQUAL_DOUBLE(0.0, r.gflops);
}

static void test_bench_gemm_i8_gflops_consistent_with_formula(void) {
    bench_result_t r = bench_gemm_i8(128, 128, 128);
    double expected = compute_gflops(128, 128, 128, r.elapsed_s);
    TEST_ASSERT_DOUBLE_WITHIN(expected * 1e-6 + 1e-12, expected, r.gflops);
}

/* -------------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------------- */

int main(void) {
    UNITY_BEGIN();

    /* Group 1: compute_gflops() */
    RUN_TEST(test_compute_gflops_1x1x1_at_1s);
    RUN_TEST(test_compute_gflops_64x64x64);
    RUN_TEST(test_compute_gflops_doubles_when_elapsed_halved);
    RUN_TEST(test_compute_gflops_zero_elapsed_returns_zero);

    /* Group 2: clocks_to_us() */
    RUN_TEST(test_clocks_to_us_zero_ticks);
    RUN_TEST(test_clocks_to_us_one_full_second);
    RUN_TEST(test_clocks_to_us_half_second);
    RUN_TEST(test_clocks_to_us_result_unit_is_microseconds);

    /* Group 3: sweep size array */
    RUN_TEST(test_bench_sweep_size_count_is_five);
    RUN_TEST(test_bench_sweep_contains_size_8);
    RUN_TEST(test_bench_sweep_contains_size_16);
    RUN_TEST(test_bench_sweep_contains_size_32);
    RUN_TEST(test_bench_sweep_contains_size_64);
    RUN_TEST(test_bench_sweep_contains_size_128);
    RUN_TEST(test_bench_sweep_sizes_are_sorted_ascending);

    /* Group 4: gemm_f32 correctness at benchmark sizes */
    RUN_TEST(test_gemm_f32_correct_at_benchmark_size_8);
    RUN_TEST(test_gemm_f32_correct_at_benchmark_size_16);
    RUN_TEST(test_gemm_f32_correct_at_benchmark_size_32);
    RUN_TEST(test_gemm_f32_correct_at_benchmark_size_64);
    RUN_TEST(test_gemm_f32_correct_at_benchmark_size_128);

    /* Group 5: gemm_i8 correctness at benchmark sizes */
    RUN_TEST(test_gemm_i8_correct_at_benchmark_size_8);
    RUN_TEST(test_gemm_i8_correct_at_benchmark_size_16);
    RUN_TEST(test_gemm_i8_correct_at_benchmark_size_32);
    RUN_TEST(test_gemm_i8_correct_at_benchmark_size_64);
    RUN_TEST(test_gemm_i8_correct_at_benchmark_size_128);

    /* Group 6: bench_gemm_f32() result sanity */
    RUN_TEST(test_bench_gemm_f32_elapsed_s_nonnegative_at_8x8);
    RUN_TEST(test_bench_gemm_f32_elapsed_s_positive_at_128x128);
    RUN_TEST(test_bench_gemm_f32_elapsed_us_matches_elapsed_s);
    RUN_TEST(test_bench_gemm_f32_gflops_nonnegative_at_128x128);
    RUN_TEST(test_bench_gemm_f32_gflops_consistent_with_formula);

    /* Group 7: bench_gemm_i8() result sanity */
    RUN_TEST(test_bench_gemm_i8_elapsed_s_nonnegative_at_8x8);
    RUN_TEST(test_bench_gemm_i8_elapsed_s_positive_at_128x128);
    RUN_TEST(test_bench_gemm_i8_gflops_nonnegative_at_128x128);
    RUN_TEST(test_bench_gemm_i8_gflops_consistent_with_formula);

    return UNITY_END();
}
