#include "../../third_party/unity/src/unity.h"

#include <stddef.h>

#include "../include/ml_kernels.h"

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

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_dot_product_f32_empty_vector_is_zero);
    RUN_TEST(test_dot_product_f32_positive_values);
    RUN_TEST(test_dot_product_f32_mixed_sign_values);
    RUN_TEST(test_dot_product_f32_fractional_values);

    return UNITY_END();
}
