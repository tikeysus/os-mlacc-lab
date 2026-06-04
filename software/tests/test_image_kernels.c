#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../software/include/image_kernels.h"

static int expect_u8_array(const char *name, const uint8_t *actual, const uint8_t *expected,
                           size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (actual[i] != expected[i]) {
            printf("%s: index %zu got %u expected %u\n", name, i, actual[i], expected[i]);
            return 1;
        }
    }

    return 0;
}

static int expect_int(const char *name, int actual, int expected) {
    if (actual != expected) {
        printf("%s: got %d expected %d\n", name, actual, expected);
        return 1;
    }

    return 0;
}

static int run_test(const char *name, int (*test_fn)(void)) {
    const int failures = test_fn();

    if (failures == 0) {
        printf("%s:PASS\n", name);
        return 0;
    }

    printf("%s:FAIL\n", name);
    return failures;
}

static int test_threshold_u8_basic(void) {
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

    return expect_u8_array("test_threshold_u8_basic", output, expected, 6);
}

static int test_threshold_u8_uses_stride(void) {
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

    return expect_u8_array("test_threshold_u8_uses_stride", output, expected, 8);
}

static int test_convolve_u8_identity_3x3(void) {
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

    return expect_u8_array("test_convolve_u8_identity_3x3", output, input, 25);
}

static int test_convolve_u8_supports_non_square_kernel(void) {
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

    int failures = expect_int("test_convolve_u8_supports_non_square_kernel_status",
                              convolve_u8(src, dst, kernel, 3, 1), IMAGE_KERNEL_OK);
    failures +=
        expect_u8_array("test_convolve_u8_supports_non_square_kernel", output, expected, 15);

    return failures;
}

static int test_convolve_u8_clamps_to_u8_range(void) {
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

    int failures = expect_int("test_convolve_u8_clamps_to_u8_range_status",
                              convolve_u8(src, dst, kernel, 3, 3), IMAGE_KERNEL_OK);
    failures += expect_u8_array("test_convolve_u8_clamps_to_u8_range", output, expected, 9);

    return failures;
}

static int test_convolve_u8_uses_stride(void) {
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

    int failures = expect_int("test_convolve_u8_uses_stride_status",
                              convolve_u8(src, dst, kernel, 1, 1), IMAGE_KERNEL_OK);
    failures += expect_u8_array("test_convolve_u8_uses_stride", output, expected, 8);

    return failures;
}

static int test_convolve_u8_rejects_bad_dimensions(void) {
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

    int failures =
        expect_int("test_convolve_u8_rejects_even_kernel", convolve_u8(src, dst, even_kernel, 2, 2),
                   IMAGE_KERNEL_INVALID_DIMENSIONS);
    failures +=
        expect_int("test_convolve_u8_rejects_large_kernel",
                   convolve_u8(src, dst, wide_kernel, 5, 3), IMAGE_KERNEL_INVALID_DIMENSIONS);

    return failures;
}

static int test_sobel3x3_u8_detects_vertical_edge(void) {
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

    return expect_u8_array("test_sobel3x3_u8_detects_vertical_edge", output, expected, 25);
}

static int test_sobel3x3_u8_applies_threshold(void) {
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

    return expect_u8_array("test_sobel3x3_u8_applies_threshold", output, expected, 25);
}

static int test_sobel3x3_u8_uses_stride(void) {
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

    return expect_u8_array("test_sobel3x3_u8_uses_stride", output, expected, 20);
}

int main(void) {
    int failures = 0;

    failures += run_test("test_threshold_u8_basic", test_threshold_u8_basic);
    failures += run_test("test_threshold_u8_uses_stride", test_threshold_u8_uses_stride);
    failures += run_test("test_convolve_u8_identity_3x3", test_convolve_u8_identity_3x3);
    failures += run_test("test_convolve_u8_supports_non_square_kernel",
                         test_convolve_u8_supports_non_square_kernel);
    failures +=
        run_test("test_convolve_u8_clamps_to_u8_range", test_convolve_u8_clamps_to_u8_range);
    failures += run_test("test_convolve_u8_uses_stride", test_convolve_u8_uses_stride);
    failures += run_test("test_convolve_u8_rejects_bad_dimensions",
                         test_convolve_u8_rejects_bad_dimensions);
    failures +=
        run_test("test_sobel3x3_u8_detects_vertical_edge", test_sobel3x3_u8_detects_vertical_edge);
    failures += run_test("test_sobel3x3_u8_applies_threshold", test_sobel3x3_u8_applies_threshold);
    failures += run_test("test_sobel3x3_u8_uses_stride", test_sobel3x3_u8_uses_stride);

    if (failures != 0) {
        printf("test_image_kernels: %d failure(s)\n", failures);
        return 1;
    }

    printf("test_image_kernels: all tests passed\n");
    return 0;
}
