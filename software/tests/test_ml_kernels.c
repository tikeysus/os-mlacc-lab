#include "unity.h"

#include <stddef.h>

#include "ml_kernels.h"

void set_up(void) {}

void tear_down(void) {}

static void test_dot_product_f32_empty_vector_is_zero(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0f, dot_product_f32(NULL, NULL, 0));
}

static void test_dot_product_f32_positive_values(void) {
    const float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};

    TEST_ASSERT_EQUAL_FLOAT(70.0f, dot_product_f32(a, b, 4));
}

static void test_dot_product_f32_mixed_sign_values(void) {
    const float a[4] = {-1.0f, 2.0f, -3.0f, 4.0f};
    const float b[4] = {5.0f, -6.0f, -7.0f, 8.0f};

    TEST_ASSERT_EQUAL_FLOAT(36.0f, dot_product_f32(a, b, 4));
}

static void test_dot_product_f32_fractional_values(void) {
    const float a[3] = {0.5f, 1.5f, -2.0f};
    const float b[3] = {2.0f, -4.0f, 0.25f};

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -5.5f, dot_product_f32(a, b, 3));
}

static void test_dot_product_i8_empty_vector_is_zero(void) {
    TEST_ASSERT_EQUAL_INT32(0, dot_product_i8(NULL, NULL, 0));
}

static void test_dot_product_i8_mixed_sign_values(void) {
    const int8_t a[5] = {-3, 2, 7, -8, 1};
    const int8_t b[5] = {4, -5, 6, -2, 9};

    TEST_ASSERT_EQUAL_INT32(45, dot_product_i8(a, b, 5));
}

static void test_dot_product_i8_accumulates_extreme_values(void) {
    const int8_t a[4] = {INT8_MIN, INT8_MAX, INT8_MIN, INT8_MAX};
    const int8_t b[4] = {INT8_MIN, INT8_MAX, INT8_MAX, INT8_MIN};

    TEST_ASSERT_EQUAL_INT32(1, dot_product_i8(a, b, 4));
}

static void test_gemv_f32_rectangular_matrix(void) {
    const float matrix[6] = {
        1.0f, 2.0f, 3.0f, -1.0f, 0.5f, 4.0f,
    };
    const float vector[3] = {2.0f, -1.0f, 0.5f};
    const float expected[2] = {1.5f, -0.5f};
    const ml_matrix_shape shape = {.rows = 2, .cols = 3};
    float output[2] = {0.0f};

    gemv_f32(matrix, vector, output, shape);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected[0], output[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected[1], output[1]);
}

static void test_gemv_f32_zero_columns_produces_zeroes(void) {
    const ml_matrix_shape shape = {.rows = 3, .cols = 0};
    float output[3] = {1.0f, 2.0f, 3.0f};

    gemv_f32(NULL, NULL, output, shape);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, output[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, output[1]);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, output[2]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_dot_product_f32_empty_vector_is_zero);
    RUN_TEST(test_dot_product_f32_positive_values);
    RUN_TEST(test_dot_product_f32_mixed_sign_values);
    RUN_TEST(test_dot_product_f32_fractional_values);
    RUN_TEST(test_dot_product_i8_empty_vector_is_zero);
    RUN_TEST(test_dot_product_i8_mixed_sign_values);
    RUN_TEST(test_dot_product_i8_accumulates_extreme_values);
    RUN_TEST(test_gemv_f32_rectangular_matrix);
    RUN_TEST(test_gemv_f32_zero_columns_produces_zeroes);

    return UNITY_END();
}
