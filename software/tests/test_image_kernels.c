#include <stdint.h>
#include <stdio.h>

#include "image_kernels.h"

static int test_threshold_u8_placeholder(void) {
    uint8_t input[4] = {0};
    uint8_t output[4] = {0};

    const image_u8_view src = {
        .data = input,
        .width = 2,
        .height = 2,
        .stride = 2,
    };
    const image_u8 dst = {
        .data = output,
        .width = 2,
        .height = 2,
        .stride = 2,
    };

    threshold_u8(src, dst, 128);

    return 0;
}

static int test_convolve3x3_u8_placeholder(void) {
    uint8_t input[25] = {0};
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

    return 0;
}

static int test_sobel3x3_u8_placeholder(void) {
    uint8_t input[25] = {0};
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

    return 0;
}

int main(void) {
    int failures = 0;

    failures += test_threshold_u8_placeholder();
    failures += test_convolve3x3_u8_placeholder();
    failures += test_sobel3x3_u8_placeholder();

    if (failures != 0) {
        printf("test_image_kernels: %d failure(s)\n", failures);
        return 1;
    }

    printf("test_image_kernels: skeleton ready\n");
    return 0;
}
