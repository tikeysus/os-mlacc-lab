#include <stdint.h>
#include <string.h>

#include "image_kernels.h"
#include "unity.h"

void set_up(void) {}

void tear_down(void) {}

static void test_threshold_u8_basic(void) {
    const uint8_t input[6] = {0, 64, 127, 128, 200, 255};
    const uint8_t expected[6] = {0, 0, 0, 255, 255, 255};
    uint8_t output[6] = {1, 1, 1, 1, 1, 1};

    const image_u8_view src = {
        .data = input,
        .width = 3,
        .height = 2,
        .stride = 3,
    };
    const image_u8 dst = {
        .data = output,
        .width = 3,
        .height = 2,
        .stride = 3,
    };

    threshold_u8(src, dst, 128);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 6);
}

static void test_threshold_u8_uses_stride(void) {
    const uint8_t input[8] = {
        10, 130, 240, 99, 127, 128, 1, 77,
    };
    const uint8_t expected[8] = {
        0, 255, 255, 42, 0, 255, 0, 42,
    };
    uint8_t output[8];

    memset(output, 42, sizeof(output));

    const image_u8_view src = {
        .data = input,
        .width = 3,
        .height = 2,
        .stride = 4,
    };
    const image_u8 dst = {
        .data = output,
        .width = 3,
        .height = 2,
        .stride = 4,
    };

    threshold_u8(src, dst, 128);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 8);
}

static void test_convolve_u8_identity_3x3(void) {
    const uint8_t input[25] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    };
    uint8_t output[25] = {0};
    const int8_t identity_kernel[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};

    const image_u8_view src = {
        .data = input,
        .width = 5,
        .height = 5,
        .stride = 5,
    };
    const image_u8 dst = {
        .data = output,
        .width = 5,
        .height = 5,
        .stride = 5,
    };

    convolve3x3_u8(src, dst, identity_kernel);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(input, output, 25);
}

static void test_convolve_u8_supports_non_square_kernel(void) {
    const uint8_t input[15] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    };
    const uint8_t expected[15] = {
        3, 6, 9, 12, 9, 13, 21, 24, 27, 19, 23, 36, 39, 42, 29,
    };
    uint8_t output[15] = {0};
    const int16_t kernel[3] = {1, 1, 1};

    const image_u8_view src = {
        .data = input,
        .width = 5,
        .height = 3,
        .stride = 5,
    };
    const image_u8 dst = {
        .data = output,
        .width = 5,
        .height = 3,
        .stride = 5,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OK, convolve_u8(src, dst, kernel, 3, 1));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 15);
}

static void test_convolve_u8_clamps_to_u8_range(void) {
    const uint8_t input[9] = {
        100, 100, 100, 100, 100, 100, 100, 100, 100,
    };
    const uint8_t expected[9] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255,
    };
    uint8_t output[9] = {0};
    const int16_t kernel[9] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    const image_u8_view src = {
        .data = input,
        .width = 3,
        .height = 3,
        .stride = 3,
    };
    const image_u8 dst = {
        .data = output,
        .width = 3,
        .height = 3,
        .stride = 3,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OK, convolve_u8(src, dst, kernel, 3, 3));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 9);
}

static void test_convolve_u8_uses_stride(void) {
    const uint8_t input[8] = {
        1, 2, 3, 99, 4, 5, 6, 99,
    };
    const uint8_t expected[8] = {
        1, 2, 3, 42, 4, 5, 6, 42,
    };
    uint8_t output[8];
    const int16_t kernel[1] = {1};

    memset(output, 42, sizeof(output));

    const image_u8_view src = {
        .data = input,
        .width = 3,
        .height = 2,
        .stride = 4,
    };
    const image_u8 dst = {
        .data = output,
        .width = 3,
        .height = 2,
        .stride = 4,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OK, convolve_u8(src, dst, kernel, 1, 1));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 8);
}

static void test_convolve_u8_rejects_bad_dimensions(void) {
    const uint8_t input[9] = {0};
    uint8_t output[9] = {0};
    const int16_t even_kernel[4] = {1, 1, 1, 1};
    const int16_t wide_kernel[15] = {0};

    const image_u8_view src = {
        .data = input,
        .width = 3,
        .height = 3,
        .stride = 3,
    };
    const image_u8 dst = {
        .data = output,
        .width = 3,
        .height = 3,
        .stride = 3,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_DIMENSIONS,
                          convolve_u8(src, dst, even_kernel, 2, 2));
    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_DIMENSIONS,
                          convolve_u8(src, dst, wide_kernel, 5, 3));
}

static void test_sobel3x3_u8_detects_vertical_edge(void) {
    const uint8_t input[25] = {
        0,   0,   255, 255, 255, 0,   0,   255, 255, 255, 0,   0,   255,
        255, 255, 0,   0,   255, 255, 255, 0,   0,   255, 255, 255,
    };
    const uint8_t expected[25] = {
        0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0,
    };
    uint8_t output[25] = {0};

    const image_u8_view src = {
        .data = input,
        .width = 5,
        .height = 5,
        .stride = 5,
    };
    const image_u8 dst = {
        .data = output,
        .width = 5,
        .height = 5,
        .stride = 5,
    };

    sobel3x3_u8(src, dst, 64);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 25);
}

static void test_sobel3x3_u8_applies_threshold(void) {
    const uint8_t input[25] = {
        0, 0, 10, 10, 10, 0, 0, 10, 10, 10, 0, 0, 10, 10, 10, 0, 0, 10, 10, 10, 0, 0, 10, 10, 10,
    };
    const uint8_t expected[25] = {0};
    uint8_t output[25];

    memset(output, 255, sizeof(output));

    const image_u8_view src = {
        .data = input,
        .width = 5,
        .height = 5,
        .stride = 5,
    };
    const image_u8 dst = {
        .data = output,
        .width = 5,
        .height = 5,
        .stride = 5,
    };

    sobel3x3_u8(src, dst, 64);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 25);
}

static void test_sobel3x3_u8_uses_stride(void) {
    const uint8_t input[20] = {
        0, 0, 255, 255, 77, 0, 0, 255, 255, 77, 0, 0, 255, 255, 77, 0, 0, 255, 255, 77,
    };
    const uint8_t expected[20] = {
        0, 0, 0, 0, 42, 0, 255, 255, 0, 42, 0, 255, 255, 0, 42, 0, 0, 0, 0, 42,
    };
    uint8_t output[20];

    memset(output, 42, sizeof(output));

    const image_u8_view src = {
        .data = input,
        .width = 4,
        .height = 4,
        .stride = 5,
    };
    const image_u8 dst = {
        .data = output,
        .width = 4,
        .height = 4,
        .stride = 5,
    };

    sobel3x3_u8(src, dst, 64);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 20);
}

static void test_image_kernels_reject_null_buffers(void) {
    const int16_t kernel[1] = {1};
    const image_u8_view src = {
        .data = NULL,
        .width = 1,
        .height = 1,
        .stride = 1,
    };
    const image_u8 dst = {
        .data = NULL,
        .width = 1,
        .height = 1,
        .stride = 1,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_ARGUMENT, threshold_u8(src, dst, 1));
    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_ARGUMENT, convolve_u8(src, dst, kernel, 1, 1));
    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_ARGUMENT, sobel3x3_u8(src, dst, 1));
}

static void test_image_kernels_reject_overflowing_stride(void) {
    const uint8_t input[1] = {1};
    uint8_t output[1] = {0};
    const image_u8_view src = {
        .data = input,
        .width = 1,
        .height = 2,
        .stride = SIZE_MAX,
    };
    const image_u8 dst = {
        .data = output,
        .width = 1,
        .height = 2,
        .stride = 1,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_INVALID_ARGUMENT, threshold_u8(src, dst, 1));
}

static void test_threshold_u8_allows_exact_in_place_operation(void) {
    const uint8_t expected[4] = {0, 0, 255, 255};
    uint8_t pixels[4] = {1, 127, 128, 255};
    const image_u8_view src = {
        .data = pixels,
        .width = 4,
        .height = 1,
        .stride = 4,
    };
    const image_u8 dst = {
        .data = pixels,
        .width = 4,
        .height = 1,
        .stride = 4,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OK, threshold_u8(src, dst, 128));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, pixels, 4);
}

static void test_image_kernels_reject_unsafe_overlap(void) {
    uint8_t pixels[18] = {0};
    const int16_t kernel[1] = {1};
    const image_u8_view src = {
        .data = pixels,
        .width = 3,
        .height = 3,
        .stride = 3,
    };
    const image_u8 shifted_dst = {
        .data = pixels + 1,
        .width = 3,
        .height = 3,
        .stride = 3,
    };
    const image_u8 same_dst = {
        .data = pixels,
        .width = 3,
        .height = 3,
        .stride = 3,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OVERLAPPING_BUFFERS, threshold_u8(src, shifted_dst, 1));
    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OVERLAPPING_BUFFERS,
                          convolve_u8(src, same_dst, kernel, 1, 1));
    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OVERLAPPING_BUFFERS, sobel3x3_u8(src, same_dst, 1));
}

static void test_sobel3x3_u8_clears_tiny_image(void) {
    const uint8_t input[2] = {100, 200};
    const uint8_t expected[2] = {0, 0};
    uint8_t output[2] = {255, 255};
    const image_u8_view src = {
        .data = input,
        .width = 2,
        .height = 1,
        .stride = 2,
    };
    const image_u8 dst = {
        .data = output,
        .width = 2,
        .height = 1,
        .stride = 2,
    };

    TEST_ASSERT_EQUAL_INT(IMAGE_KERNEL_OK, sobel3x3_u8(src, dst, 1));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 2);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_threshold_u8_basic);
    RUN_TEST(test_threshold_u8_uses_stride);
    RUN_TEST(test_convolve_u8_identity_3x3);
    RUN_TEST(test_convolve_u8_supports_non_square_kernel);
    RUN_TEST(test_convolve_u8_clamps_to_u8_range);
    RUN_TEST(test_convolve_u8_uses_stride);
    RUN_TEST(test_convolve_u8_rejects_bad_dimensions);
    RUN_TEST(test_sobel3x3_u8_detects_vertical_edge);
    RUN_TEST(test_sobel3x3_u8_applies_threshold);
    RUN_TEST(test_sobel3x3_u8_uses_stride);
    RUN_TEST(test_image_kernels_reject_null_buffers);
    RUN_TEST(test_image_kernels_reject_overflowing_stride);
    RUN_TEST(test_threshold_u8_allows_exact_in_place_operation);
    RUN_TEST(test_image_kernels_reject_unsafe_overlap);
    RUN_TEST(test_sobel3x3_u8_clears_tiny_image);

    return UNITY_END();
}
