#include <stdint.h>
#include <stdio.h>

#include "../../include/image_kernels.h"

int main(void) {
    uint8_t input[64] = {0};
    uint8_t output[64] = {0};

    const image_u8_view src = {
        .data = input,
        .width = 8,
        .height = 8,
        .stride = 8,
    };
    const image_u8 dst = {
        .data = output,
        .width = 8,
        .height = 8,
        .stride = 8,
    };

    sobel3x3_u8(src, dst, 64);

    printf("edge_detection_demo: skeleton ready\n");

    return 0;
}
