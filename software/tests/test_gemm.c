#include "unity.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "ml_kernels.h"

void set_up(void) {}

void tear_down(void) {}

static void test_gemm_f32_1x1(void) {
    const float a[1] = {3.0f};
    const float b[1] = {4.0f};
    float c[1] = {0.0f};

    gemm_f32(a, b, c, 1, 1, 1);

    TEST_ASSERT_EQUAL_FLOAT(12.0f, c[0]);
}

static void test_gemm_f32_2x2(void) {
    const float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    float c[4] = {0.0f};

    gemm_f32(a, b, c, 2, 2, 2);

    TEST_ASSERT_EQUAL_FLOAT(19.0f, c[0]);
    TEST_ASSERT_EQUAL_FLOAT(22.0f, c[1]);
    TEST_ASSERT_EQUAL_FLOAT(43.0f, c[2]);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, c[3]);
}

static void test_gemm_f32_non_square(void) {
    /* a is 2x3, b is 3x4 — identity-like rows make expected values easy to verify */
    const float a[6] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    const float b[12] = {
        1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f,
    };
    float c[8] = {0.0f};

    gemm_f32(a, b, c, 2, 3, 4);

    TEST_ASSERT_EQUAL_FLOAT(1.0f, c[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, c[1]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, c[2]);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, c[3]);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, c[4]);
    TEST_ASSERT_EQUAL_FLOAT(6.0f, c[5]);
    TEST_ASSERT_EQUAL_FLOAT(7.0f, c[6]);
    TEST_ASSERT_EQUAL_FLOAT(8.0f, c[7]);
}

static void test_gemm_f32_zero_matrix(void) {
    const float a[9] = {0.0f};
    const float b[9] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    float c[9] = {0.0f};

    gemm_f32(a, b, c, 3, 3, 3);

    for (int i = 0; i < 9; i++)
        TEST_ASSERT_EQUAL_FLOAT(0.0f, c[i]);
}

static void test_gemm_i8_2x2(void) {
    const int8_t a[4] = {1, 2, 3, 4};
    const int8_t b[4] = {5, 6, 7, 8};
    int32_t c[4] = {0};

    gemm_i8(a, b, c, 2, 2, 2);

    TEST_ASSERT_EQUAL_INT32(19, c[0]);
    TEST_ASSERT_EQUAL_INT32(22, c[1]);
    TEST_ASSERT_EQUAL_INT32(43, c[2]);
    TEST_ASSERT_EQUAL_INT32(50, c[3]);
}

static void test_gemm_i8_extreme_values_fit_int32(void) {
    /* k=4 of INT8_MAX squared: 4 * 127 * 127 = 64516, well within int32 range */
    const int8_t a[4] = {INT8_MAX, INT8_MAX, INT8_MAX, INT8_MAX};
    const int8_t b[4] = {INT8_MAX, INT8_MAX, INT8_MAX, INT8_MAX};
    int32_t c[1] = {0};

    gemm_i8(a, b, c, 1, 4, 1);

    TEST_ASSERT_EQUAL_INT32(4 * (int32_t)INT8_MAX * (int32_t)INT8_MAX, c[0]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_gemm_f32_1x1);
    RUN_TEST(test_gemm_f32_2x2);
    RUN_TEST(test_gemm_f32_non_square);
    RUN_TEST(test_gemm_f32_zero_matrix);
    RUN_TEST(test_gemm_i8_2x2);
    RUN_TEST(test_gemm_i8_extreme_values_fit_int32);

    return UNITY_END();
}
